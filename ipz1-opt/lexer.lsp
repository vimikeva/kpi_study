3;; grammar signal [7]

#| 
0 - whitespace
1 - constants
2 - id and keyword
3 - one-character delimiters
4 - a lot of character delimiters
5 - comment
6 - invalid characters
7 - end of file (code 26)
|#

(defpackage :lexer
  (:use :cl :asdf))

(in-package :lexer)

(defparameter *col* 0)
(defparameter *row* 1)

(defparameter *shift-keywords* 300)
(defparameter *shift-const* 800)
(defparameter *shift-id* 2000)

(defparameter *keywords* '("PROGRAM" "PROCEDURE" "BEGIN" "END" "CONST" "VAR"
                           "COMPLEX" "INTEGER" "FLOAT" "BLOGFLOAT" "EXT"
                           "DEFFUNC" "WHILE" "DO" "ENDWHILE" "LOOP" "ENDLOOP"
                           "FOR" "ENDFOR" "CASE" "OF" "ENDCASE" "RETURN"
                           "IF" "THEN" "ELSE" "TO" "DO" "OR" "AND" "NOT" "MOD"
                           "$EXP"))

(defparameter *delimiters* '(";.,-+*/\:()[]<>=!&^\"'#\\"
                             (":=" "($" "$)" "<=" "<>" ">=")))

(defparameter *comment-chars* '("(*" "*)"))

(defparameter *ascii-code-table* (make-array '(256)))

(defparameter *keyword-and-alocd-table* (make-hash-table))
(defparameter *identifiers* (make-hash-table))
(defparameter *constants* (make-hash-table))



(defun aref-ascii (code)
  (if (< code 256)
      (aref *ascii-code-table* code)
      6))


(defun initialization-tables ()
  ;; clean
  (setf *col* 0)
  (setf *row* 1)
  (setf *ascii-code-table*
        (adjust-array *ascii-code-table* '(256)
                      :initial-element 6))
  (setf *keyword-and-alocd-table* (make-hash-table))
  (setf *identifiers* (make-hash-table))
  (setf *constants* (make-hash-table))
  
  ;; init *ascii-code table*
  (dotimes (i 33 t)
    (setf (aref *ascii-code-table* i) 0))

  (do ((i 48 (1+ i)))
      ((eql i 59) t)
    (setf (aref *ascii-code-table* i) 1))

  (do ((i 65 (1+ i))
       (j 97 (1+ j)))
      ((eql i 91) t)
    (setf (aref *ascii-code-table* i) 2)
    (setf (aref *ascii-code-table* j) 2))

  (dolist (i (coerce (car *delimiters*) 'list))
    (setf (aref *ascii-code-table* (char-code i)) 3))

  (dolist (i (second *delimiters*))
    (setf (aref *ascii-code-table* (char-code (char i 0))) 3))

  (setf (aref *ascii-code-table* (char-code #\()) 3)
  (setf (aref *ascii-code-table* (char-code #\Sub)) 7) ;eof

  ;; init *keyword-and-alocd-table*
  (dolist (elem *keywords*)
    (setf (gethash (+ *shift-keywords* (hash-table-count *keyword-and-alocd-table*))
                   *keyword-and-alocd-table*) elem))
  
  (dolist (elem (cadr *delimiters*))
    (setf (gethash (+ *shift-keywords* (hash-table-count *keyword-and-alocd-table*))
                   *keyword-and-alocd-table*) elem))
  (setf (gethash (+ *shift-keywords* (hash-table-count *keyword-and-alocd-table*))
                 *keyword-and-alocd-table*) "ENDIF")
  )


(defun scan (stream)
  (let ((sym (read-char stream nil #\Sub)))
    (cond
      ((char-equal sym #\Sub) sym)
      ((char-equal sym #\Newline)
       (incf *row*)
       (setf *col* 0)
       sym)
      (t (incf *col*)
         sym))))


(defun const-token-back (const pos &aux token)
  (maphash (lambda (key val)
             (when (string-equal const val)
               (setq token key)))
           *constants*)
  (unless token
    (setf token (+ *shift-const* (hash-table-count *constants*)))
    (setf (gethash token *constants*) const))
  `(,pos (,token ,const)))


(defun id-token-back (id pos &aux token)
  (maphash (lambda (key val)
             (when (string-equal id val)
               (setq token key)))
           *keyword-and-alocd-table*)
  (maphash (lambda (key val)
             (when (string-equal id val)
               (setq token key)))
           *identifiers*)
  (unless token
    (setf token (+ *shift-id* (hash-table-count *identifiers*)))
    (setf (gethash token *identifiers*) id))
  `(,pos (,token ,id)))


(defun error-token-back (syms pos)
  `(,pos (err ,syms)))


(defun whitespace-token (stream token-list)
  (let ((sym (scan stream)))
    (if (eql 0 (aref-ascii (char-code sym)))
        (whitespace-token stream token-list)
        (qualifier stream sym token-list))))


(defun comment-delete (stream &optional sym &aux end-sym)
  (cond
    ((char-equal sym #\Sub)
     sym)
    ((char-equal sym #\*)
     (setq end-sym (scan stream))
     (if (char-equal end-sym #\))
         (scan stream)
         (comment-delete stream end-sym)))
    (t (comment-delete stream (scan stream)))))


(defun separator-token (stream syms pos token-list)
  (push (scan stream) syms)
  (let ((str-token (concatenate 'string (reverse syms)))
        token)
    (maphash (lambda (key val)
               (when (string-equal val str-token)
                 (setq token key)))
             *keyword-and-alocd-table*)
    (cond
      (token
           (qualifier stream (scan stream)
                      (cons `(,pos (,token ,str-token))
                            token-list)))
      ((string-equal (car *comment-chars*) str-token)
       (qualifier stream
                  (comment-delete stream (scan stream))
                  token-list))
      ((string-equal str-token "$E")
       (character-token stream syms pos token-list))
      (t
       (qualifier stream (car syms)
                  (cons `(,pos (,(char-code (second syms))
                                ,(second syms)))
                        token-list))))))


(defun digit-token (stream syms pos token-list)
  (let ((sym-next (scan stream)))
    (if (eql 1 (aref-ascii (char-code sym-next)))
        (digit-token stream (cons sym-next syms) pos token-list)
        (qualifier stream
                   sym-next
                   (cons (const-token-back (concatenate 'string
                                                          (reverse syms))
                                             pos)
                           token-list)))))

(defun character-token (stream syms pos token-list)
  (let ((sym-next (scan stream)))
    (if (member (aref-ascii (char-code sym-next)) '(1 2))
        (character-token stream (cons sym-next syms) pos token-list)
        (qualifier stream
                   sym-next
                   (cons (id-token-back (concatenate 'string
                                                     (reverse syms))
                                        pos)
                         token-list)))))


(defun qualifier (stream sym token-list)
  (let ((symb (aref-ascii (char-code sym))))
    (cond
      ((eql 0 symb)
       (whitespace-token stream token-list))
      ((eql 1 symb)
       (digit-token stream (list sym)
                    (list *row* *col*) token-list))
      ((eql 2 symb)
       (character-token stream (list sym)
                        (list *row* *col*) token-list))
      ((eql 3 symb)
       (separator-token stream (list sym)
                        (list *row* *col*) token-list))
      ((eql 6 symb)
       (qualifier stream
                  (scan stream)
                  (cons (error-token-back sym (list *row* *col*))
                        token-list)))
      ((eql 7 symb)
       (reverse token-list)))))


(defun lexer (file-name)
  (initialization-tables)
  (with-open-file (stream file-name)
    (qualifier stream (scan stream) nil)))


(defun print-lexems (token-list &optional (stream t))
  (labels ((%print (token-list)
             (when token-list
               (destructuring-bind ((row col) (token str)) (car token-list)
                 (format stream "~A:~A   " row col)
                 (format stream "~A:  \"~A\"~%" token str))
               (%print (cdr token-list)))))
    
    (format stream "token list:~%")
    (%print token-list)))
    

;; (with-open-file (stream "res.test.1"
;;                         :direction :output
;;                         :if-exists :supersede)
;;   (print-lexems (lexer "tests/test.1") stream))

;; (print-lexems (lexer "tests/test.1"))

