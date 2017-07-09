;; grammar signal[7]

(defpackage :parser
  (:use :cl :asdf :lexer))

(in-package :parser)


;;; define macro for simple work with grammar :)

(defmacro define-rule-simple (rule-name (&rest rule-list) (&rest name-list))
  (labels ((%. (rule-simple name-simple &optional (num 1) tree)
             (push (concatenate 'string "TREE-" (write-to-string num)) tree)
             (if rule-simple
                 `(if tokens
                      ,(if (numberp (car rule-simple))
                           `(if (eql ,(car rule-simple) (casar tokens))
                                (multiple-value-bind (,(intern (car tree)) tokens)
                                    (values (define-rule (car tokens) ,(car name-simple))
                                            (cdr tokens))
                                  ,(%. (cdr rule-simple) (cdr name-simple) (1+ num) tree))
                                (make-rule :name-node ,rule-name
                                           :children (list ,@(mapcar 'intern (reverse (cdr tree)))
                                                           ;; (error-rule (car tokens))
                                                           )) )
                           `(multiple-value-bind (,(intern (car tree)) tokens)
                                (,(car rule-simple) tokens)
                              ,(%. (cdr rule-simple) (cdr name-simple) (1+ num) tree)) )
                      (make-rule :name-node ,rule-name
                                 :children (list ,@(mapcar 'intern (reverse (cdr tree))))) )
                 `(values (make-rule :name-node ,rule-name
                                     :children (list ,@(mapcar 'intern (reverse (cdr tree)))))
                          tokens) ))

           (%.. (rule-list name-list)
             `(multiple-value-bind (tr tk)
                  ,(%. (car rule-list) (car name-list))
                (print-tree tr)
                (format t "~A~%~A~%=====~%" ',rule-list tk)
                ,(if (cdr rule-list)
                     `(if tk
                          (values tr tk)
                          ,(%.. (cdr rule-list) (cdr name-list)))
                     `(values tr tk)))) )
  
    (%.. rule-list name-list) ))


(defmacro defrule (rule-name (&rest rule-list) &optional name-list)
  `(defun ,(intern (concatenate 'string (symbol-name rule-name) "-RULE")) (tokens)
     (define-rule-simple ,rule-name
         ,(mapcar (lambda (rl-l)
                    (mapcar (lambda (rl)
                              (if (numberp rl)
                                  rl
                                  (intern
                                   (concatenate 'string
                                                (symbol-name rl)
                                                "-RULE"))))
                            rl-l))
           rule-list)
         ,name-list)))


;;; struct rule

(defstruct rule
  token
  node
  position
  name-node
  children)

;;; functions of grammar

(defun casar (element)
  (car
   (second
    (car element))))


(defun error-rule (&optional token)
  (if token
      (make-rule :token (car (second token))
                 :node (second (second token))
                 :position (car token)
                 :name-node :error)
      (make-rule :name-node :error)))


(defun define-rule (token rule)
  (if token
    (make-rule :token (car (second token))
               :node (second (second token))
               :position (car token)
               :name-node rule)
    (error-rule)))

(defun empty-rule (tokens)
  (if tokens
      (values (make-rule :name-node :empty)
              tokens)
      (error-rule)))

;;; simple rules of grammar

(defun unsigned-integer-rule (tokens)
  (if (and tokens
           (<= lexer::*shift-const* (casar tokens))
           (> lexer::*shift-id* (casar tokens)))
      
      (values (define-rule (car tokens) :unsigned-integer)
              (cdr tokens))
      (make-rule :name-node :unsigned-integer
                 :children (list (error-rule (car tokens)))) ))

(defun identifier-rule (tokens)
  (if (and tokens
           (<= lexer::*shift-id* (casar tokens)))
      (values (define-rule (car tokens) :identifier)
              (cdr tokens))
      (make-rule :name-node :identifier
                 :children (list (error-rule (car tokens))))))


;;; other rules of grammar

(defrule :sing
    ((43)
     (45)
     (empty))
    ((:symbol)
     (:symbol)))

(defrule :fractional-part
    ((35 sing unsigned-integer)
     (empty))   
    ((:symbol)))

(defrule :integer-part
    ((unsigned-integer)))

(defrule :unsigned-number
    ((integer-part fractional-part)))


(defrule :assembly-insert-file-identifier
  ((identifier)))

(defrule :builtin-function-identifier
    ((identifier)))

(defrule :function-identifier
    ((identifier)))

(defrule :procedure-identifier
    ((identifier)))

(defrule :variable-identifier
    ((identifier)))

(defrule :constant-identifier
    ((identifier)))

(defrule :right-part
    ((44 expression)
     (332 40 expression 41)
     (empty))
    ((:symbol)
     (:keyword :symbol nil :symbol)))

(defrule :left-part
    ((expression)
     (empty)))

(defrule :complex-number
    ((left-part right-part)))

(defrule :unsigned-constant
    ((unsigned-number)))

(defrule :complex-constant
    ((39 complex-number 39))
    ((:symbol nil :symbol)))

(defrule :expression-list
    ((44 expression expression-list)
     (empty))
    ((:symbol)))

(defrule :dimension
    ((91 expression expression-list 93)
     (empty))
    ((:symbol nil nil :symbol)))

(defrule :complex-variable
    ((34 complex-number 34))
    ((:symbol nil :symbol)))

(defrule :variable
    ((variable-identifier dimension)
     (complex-variable)))

(defrule :actual-arguments-list
    ((44 expression actual-arguments-list)
     (empty))
    ((:symbol)))

(defrule :actual-arguments
    ((40 expression actual-arguments-list 41)
     (empty))
    ((:symbol nil nil :symbol)))

(defrule :multiplier
    ((unsigned-constant)
     (complex-constant)
     (constant-identifier)
     (variable)
     (function-identifier)
     (builtin-function-identifier actual-arguments)
     (40 expression 41)
     (45 multiplier)
     (94 multiplier))
    (nil nil nil nil nil nil
         (:symbol nil :symbol)
         (:symbol)
         (:symbol)))

(defrule :multiplication-instruction
    ((42)
     (47)
     (38)
     (331))
    ((:mul-op)
     (:mul-op)
     (:mul-op)
     (:mul-op)))

(defrule :multipliers-list
    ((multiplication-instruction multiplier multipliers-list)
     (empty)))

(defrule :summand
    ((multiplier multipliers-list)))

(defrule :add-instruction
    ((43)
     (45)
     (33))
    ((:add-op)
     (:add-op)
     (:add-op)))

(defrule :summands-list
    ((add-instruction summand summands-list)
     (empty)))

(defrule :expression
    ((summand summands-list)
     (45 summand summands-list))
    (nil
     (:sumbol)))

(defrule :comparison-operator
    ((60)
     (336)
     (61)
     (337)
     (338)
     (62))
    ((:cmp-op)
     (:cmp-op)
     (:cmp-op)
     (:cmp-op)
     (:cmp-op)
     (:cmp-op)))

(defrule :logical-multiplier
    ((expression comparison-operator expression)
     (91 conditional-expression 93)
     (330 logical-multiplier))
    (nil
     (:symbol nil :symbol)
     (:logic-op)))

(defrule :logical-multipliers-list
    ((329 logical-multiplier logical-multipliers-list)
     (empty))
    ((:logic-op)))

(defrule :logical-summand
    ((logical-multiplier logical-multipliers-list)))

(defrule :logical
    ((328 logical-summand logical)
     (empty))
    ((:logic-op)))

(defrule :conditional-expression
    ((logical-summand logical)))

(defrule :alternative
    ((expression 58 47 statements-list 92))
    ((nil :symbol :symbol nil :symbol)))

(defrule :alternatives-list
    ((alternative alternatives-list)
     (empty)))

(defrule :loop-declaration
    ((expression 326 expression 327 statements-list))
    ((nil :keyword nil :keyword)))

(defrule :alternative-part
    ((325 statements-list)
     (empty))
    ((:keyword)))

(defrule :incomplete-condition-statement
    ((323 conditional-expression 324 statements-list))
    ((:keyword nil :keyword)))

(defrule :condition-statement
    ((incomplete-condition-statement alternative-part)))

(defrule :statement
    ((variable 333 expression 59)
     (procedure-identifier actual-arguments 59)
     (condition-statement 339 59)
     (312 conditional-expression 327 statements-list 314 59)
     (315 statements-list 316 59)
     (317 variable-identifier 333 loop-declaration 318 59)
     (319 expression 320 alternatives-list 321 59)
     (322 59)
     (59)
     (334 assembly-insert-file-identifier 46 identifier 335))
    ((nil :keyword nil :symbol)
     (nil nil :symbol)
     (nil :keyword :symbol)
     (:keyword nil :keyword nil :keyword :symbol)
     (:keyword nil :keyword :symbol)
     (:keyword nil :keyword nil :keyword :symbol)
     (:keyword nil :keyword nil :keyword :symbol)
     (:keyword :symbols)
     (:symbol)
     (:keyword nil :keyword)))

(defrule :statements-list
    ((statement statements-list)
     (empty)))

(defrule :parameters-list
    ((40 declarations-list 41)
     (empty))
    ((:symbol nil :symbol)))

(defrule :procedure
    ((301 procedure-identifier parameters-list 59))
    ((:keyword nil nil :symbol)))

(defrule :procedure-declarations
    ((procedure procedure-declarations)
     (empty)))

(defrule :function-characteristic
    ((92 unsigned-integer 44 unsigned-integer))
    ((:symbol nil :symbol)))

(defrule :function
    ((function-identifier 61 expression function-characteristic 59))
    ((nil :symbol nil nil :symbol)))

(defrule :function-list
    ((function function-list)
     (empty)))

(defrule :math-function-declarations
    ((311 function-list)
     (empty))
    ((:keyword)))

(defrule :attribute
    ((306)
     (307)
     (308)
     (309)
     (310))
    ((:keyword)
     (:keyword)
     (:keyword)
     (:keyword)
     (:keyword)))

(defrule :attributes-list
    ((attribute attributes-list)
     (empty)))

(defrule :identifiers-list
    ((44 variable-identifier identifiers-list)
     (empty))
    ((:symbol)))

(defrule :declaration
    ((variable-identifier identifiers-list 58 attribute attributes-list 59))
    ((nil nil :symbol nil nil :symbol)))

(defrule :declarations-list
    ((declaration declarations-list)
     (empty)))

(defrule :variable-declarations
    ((305 declarations-list)
     (empty))
    ((:keyword)))

(defrule :constant
    ((complex-constant)
     (unsigned-constant)
     (45 unsigned-constant))
    (nil nil (:symbol)))

(defrule :constant-declaration
    ((constant-identifier 61 constant 59))
    ((nil :symbol nil :symbol)))

(defrule :constant-declarations-list
    ((constant-declaration constant-declarations-list)
     (empty)))

(defrule :constant-declarations
    ((304 constant-declarations-list)
     (empty))
    ((:keyword)))

(defrule :declarations
    ((constant-declarations variable-declarations
                            math-function-declarations
                            procedure-declarations)))

(defrule :block
    ((declarations 302 statements-list 303))
    ((nil :keyword nil :keyword)))

(defrule :program
    ((300 procedure-identifier 59 block 46)
     (301 procedure-identifier parameters-list 59 block 59))
    ((:keyword nil :symbol nil :symbol)
     (:keyword nil nil :symbol nil :symbol)))

(defun signal-program-rule (tokens)
  (program-rule tokens))


;;; print parse tree

(defun print-tree (tree &optional (pr ""))
  (when tree
    (let ((vertex (if (rule-token tree)
                      (format nil " ~A ~A" (rule-node tree) (rule-position tree))
                      "")))
      (format t "~A<~A~A~%" pr (rule-name-node tree) vertex)
      (mapc (lambda (child)
              (print-tree child (format nil "| ~A" pr)))
            (rule-children tree))
      ;; (format t "~A>~%" pr)
      )))


(defun parser ()
  (print-tree
   (signal-program-rule
    (lexer::lexer "tests/test.2")))
  nil)
