;;; lab_9

(defparameter circuit '((C1 0.000002) (C2 0.000005) (L3 75) (L4 2) (L5 56)))

(defun calculation-circuit (circuit w)
  (labels ((%search-r (circuit r)
             (if (and circuit (eq r (caar circuit)))
                 (cadar circuit)
                 (%search-r (cdr circuit) r)))
           
           (%parallel (op1 op2)
             (/ (* op1 op2) (* op1 op2)))

           (%successively (op1 op2)
             (+ op1 op2)))
    

    (let* ((r-list (mapcar (lambda (elem)
                             (if (member (car elem) '(c1 c2))
                                 `(,(car elem) ,(complex (/ -1 (* w (second elem))) 0))
                                 `(,(car elem) ,(complex 0 (* w (second elem))))))
                           circuit))
           (c1 (%search-r r-list 'c1))
           (c2 (%search-r r-list 'c2))
           (l3 (%search-r r-list 'l3))
           (l4 (%search-r r-list 'l4))
           (l5 (%search-r r-list 'l5)))
      (%successively
       l5 (%parallel
           l4 (%successively
               l3 (%parallel c1 c2)))) )))




(defun main (&optional (w '(0.01 0.1 10 100)))
  (when w
    (cons `((w ,(car w)) ,(calculation-circuit circuit (car w)))
          (main (cdr w)))))

;;; testing 
;;(main)
;;(((W 0.01) #C(1.0 0.56))
;; ((W 0.1) #C(1.0 5.6))
;; ((W 10) #C(1.0 560.0)
;; ((W 100) #C(1.0 5600.0)))

