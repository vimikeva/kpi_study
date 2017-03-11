
;; 1. Create list with 4 elements using CONS
(cons 1 (cons 2 (cons 3 (cons 4 nil)))) ; ==> (1 2 3 4)

;; 2. Create list with 4 elements using LIST
(list 1 2 3 4) ; ==> (1 2 3 4)

;; 3.
(car '(a b c)) ; ==> A

;; 4.
(cdr '(a b c)) ; ==> (B C)

;; 5.
(car (cdr (cdr '(a b c d e)))) ; ==> C

;; 6.
(caddr '(a b c d e)) ; ==> C

;; 7.
(eq 'x 'x) ; ==> T
(eq '(a b) '(a b)) ; ==> NIL
(eql 3 3) ; ==> T
(eql 2.0 2) ; ==> NIL
(equal 'x 'x) ; ==> T
(equal '(a b c) '(a b c)) ; ==> T

;; 8.
(null '()) ; ==> T

;; 9.
(atom 'x) ; ==> T
(atom nil) ; ==> NIL

;; 10.
(car '(cdr '(cons 'a '(b c)))) ; ==> CDR

;; 11. with list '(a b c d) get result '(c a b d) using basic function
(defparameter lst '(a b c d))
(list (caddr lst) (car lst) (cadr lst) (cadddr lst)) ; ==> (C A B D)

