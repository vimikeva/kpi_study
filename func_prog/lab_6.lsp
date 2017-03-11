
;; 4. Написати функцію, яка обчислює суму попарних доданків двох цілочисельних списків.

(defun add-list (lst-one lst-two)
  (cond
    ((and lst-one lst-two)
       (cons (+ (car lst-one)
                (car lst-two))
             (add-list (cdr lst-one)
                       (cdr lst-two))))
    (lst-one)
    (lst-two)))

(add-list '(1 2 3 4) '(4 3 2 1)) ; ==> (5 5 5 5)
(add-list '(1 2 3 4) '(4 3)) ; ==> (5 5 3 4)
(add-list '(1) '(4 3 2 1)) ; ==> (5 3 2 1)
