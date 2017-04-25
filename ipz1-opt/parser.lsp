;; grammar signal[7]

(defpackage :parser
  (:use :cl :asdf :lexer))

(in-package :parser)




(defmacro mv-let* (binds &body body)
  (labels ((%expand (bnd bd)
             (when bnd
               `(multiple-value-bind ,(first (first bnd)) ,(second (first bnd))
                  ,@(if (rest bnd)
                        (list (%expand (cdr bnd) bd))
                        bd)))))
    (%expand binds body)))



;; functions of grammar

(defstruct rule
  token
  node
  position
  name-node
  children)

(defun error-rule (token)
  (make-rule :token (car (second token))
             :node (second (second token))
             :position (car token)
             :name-node 'error))

(defun keyword-rule (token)
  (make-rule :token (car (second token))
             :node (second (second token))
             :position (car token)
             :name-node 'keyword))

(defun if-rule (tokens tok)
  (if (eql tok (car (second (car tokens))))
      (values
       (keyword-rule (car tokens))
       (cdr values))
      (error-rule (car tokens))))


(defun identifier-rule (token)
  (make-rule :token (car (second token))
             :node (second (second token))
             :position (car token)
             :name-node 'identifier))

(defun constant-declaration-rule (tokens)
  (mv-let* ((tree-1 tokens))))

(defun constant-declarations-list-rule (tokens-st)
  (mv-let* (((tree-1 tokens) (constant-declaration-rule tokens-st))
            ((tree-2 tokens) (constant-declarations-list-rule tokens)))
           (if tokens
               (values (list tree-1 tree-2) tokens)
               tokens-st)))


(defun constant-declarations-rule (tokens-st)
  (mv-let* (((tree-1 tokens) (if-rule tokens-st))
            ((tree-2 tokens) (constant-declarations-list-rule tokens)))
           (if tokens
               (values (list tree-1 tree-2) tokens)
               tokens-st)))


(defun declarations-rule (tokens)
  (mv-let* (((tree-1 tokens) (constant-declarations-rule tokens))
            ((tree-2 tokens) (variable-declarations-rule tokens))
            ((tree-3 tokens) (math-function-declarations-rule tokens))
            ((tree-4 tokens) (procedure-declarations-rule tokens)))
           (values
            (list tree-1 tree-2 tree-3 tree-4)
            tokens)))


(defun block-rule (tokens)
  (mv-let* (((tree-1 tokens) (declaretions-rule token))
            ((tree-2 tokens) (if-rule tokens 302))
            ((tree-3 tokens) (statement-list-rule tokens))
            ((tree-4 tokens) (if-rule tokens 303)))
           (values (list tree-1 tree-2 tree-3 tree-4)
                   tokens)))


(defun procedure-identifier-rule (tokens)
  (values
   (identifier (car tokens))
   (cdr tokens)))


(defun program (tokens)
  (cond
    ((eql 300 (car (second (car tokens))))
     (mv-let* (((tree-1 tokens) (if-rule tokens 300))
               ((tree-2 tokens) (procedure-identifier-rule tokens))
               ((tree-3 tokens) (if-rule tokens 59))
               ((tree-4 tokens) (block-rule tokens)))
              (values
               (list tree-1 tree-2 tree-3 tree-4)
               tokens)))
    ((eql 301 (car (second (car tokens))))
     (mv-let* (((tree-1 tokens) (if-rule tokens 301))
               ((tree-2 tokens) (procedure-identifier-rule tokens))
               ((tree-3 tokens) (parameter-list-rule tokens))
               ((tree-4 tokens) (if-rule tokens 59))
               ((tree-5 tokens) (block-rule tokens))
               ((tree-6 tokens) (if-rule tokens 59)))
              (values
               (list tree-1 tree-2 tree-3 tree-4 tree-5 tree-6)
               tokens)))
    (t (error-rule (car tokens)))))


(defun signal-program (tokens)
  (program tokens))



(((1 1) (300 "program")) ((1 9) (2000 "pr")) ((1 11) (59 #\;)))
