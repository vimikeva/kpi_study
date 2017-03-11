;; remove from list (a b c d e f g..) odd element position

(defun remove-odd (lst)
  (setq lst (cdr lst))
  (when lst
    (cons (car lst) (remove-odd (cdr lst)))))

(remove-odd '()) ; ==> NIL
(remove-odd '(1)) ; ==> NIL
(remove-odd '(1 2 3 4 5)) ; ==> (2 4)
(remove-odd '(1 2 3 4 5 6)) ; ==> (2 4 6)

