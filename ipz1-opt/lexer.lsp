;; grammar signal [7]

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
  (:use :common-lisp))

(in-package :lexer)

(defparameter *col* 0)
(defparameter *row* 1)

(defparameter *keywords* '("PROGRAM" "PROCEDURE" "BEGIN" "END" "CONST" "VAR"
                           "COMPLEX" "INTEGER" "FLOAT" "BLOGFLOAT" "EXT"
                           "DEFFUNC" "WHILE" "DO" "ENDWHILE" "LOOP" "ENDLOOP"
                           "FOR" "ENDFOR" "CASE" "OF" "ENDCASE" "RETURN"
                           "IF" "THEN" "ELSE" "TO" "DO" "OR" "AND" "NOT" "MOD"
                           "$EXP"))

(defparameter *delimiters* '(";.,-+*/\:()[]<>=!&^\"'#"
                             (":=" "($" "$)" "<=" "<>" ">=")))

(defparameter *comment-chars* '("(*" "*)"))

(defparameter *ascii-code-table* (make-array '(256)))

(defparameter *keyword-and-alocd-table* (make-hash-table))
(defparameter *identifiers* (make-hash-table))
(defparameter *constants* (make-hash-table))




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
    (setf (aref *ascii-code-table* (char-code (char i 0))) 4))

  (setf (aref *ascii-code-table* (char-code #\()) 5)
  (setf (aref *ascii-code-table* (char-code #\Sub)) 7) ;eof

  ;; init *keyword-and-alocd-table*
  (dolist (elem *keywords*)
    (setf (gethash (hash-table-count *keyword-and-alocd-table*)
                   *keyword-and-alocd-table*) elem))
  
  (dolist (elem (cadr *delimiters*))
    (setf (gethash (hash-table-count *keyword-and-alocd-table*)
                   *keyword-and-alocd-table*) elem)) )


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
    (setf token (hash-table-count *constants*))
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
    (setf token (hash-table-count *identifiers*))
    (setf (gethash token *identifiers*) id))
  `(,pos (,token ,id)))


(defun error-token-back (syms pos)
  `(,pos (err ,syms)))


(defun whitespace-token (stream token-list)
  (let ((sym (scan stream)))
    (if (eql 0 (aref *ascii-code-table* (char-code sym)))
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
  (let ((str-token (concatenate 'string (reverse syms)))
        token)
    (push (scan stream) syms)
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
                  (cons `(,pos (,(char-code (second syms)))) token-list))))))


(defun digit-token (stream syms pos token-list)
  (let ((sym-next (scan stream)))
    (if (eql 1 (aref *ascii-code-table* (char-code sym-next)))
        (digit-token stream (cons sym-next syms) pos token-list)
        (progn 
          (unless (eql 0 (aref *ascii-code-table* (char-code sym-next)))
            (push (error-token-back sym-next pos) token-list))
          (qualifier stream
                     (scan stream)
                     (cons (const-token-back (concatenate 'string
                                                          (reverse syms))
                                             pos)
                           token-list))))))

(defun character-token (stream syms pos token-list)
  (let ((sym-next (scan stream)))
    (if (member (aref *ascii-code-table* (char-code sym-next)) '(1 2))
        (character-token stream (cons sym-next syms) pos token-list)
        (qualifier stream
                   sym-next
                   (cons (id-token-back (concatenate 'string
                                                     (reverse syms))
                                        pos)
                         token-list)))))


(defun qualifier (stream sym token-list)
  (let ((symb (aref *ascii-code-table* (char-code sym))))
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
      ((eql 4 symb)
       (separator-token stream (list sym)
                        (list *row* *col*) token-list))
      ((eql 6 symb)
       (qualifier stream
                  (scan stream)
                  (error-token-back sym (list *row* *col*))))
      ((eql 7 symb)
       (reverse token-list)))))


(defun lexer (file-name)
  (initialization-tables)
  (with-open-file (stream file-name)
    (qualifier stream (scan stream) nil)))


(print (lexer "/media/vimikeva/MediaWin/Study/kpi_study/ipz1-opt/test.1"))
