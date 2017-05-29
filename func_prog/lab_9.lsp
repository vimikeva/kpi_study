;;; lab_9

(defparameter circuit '(par (R1 150)
                        (seq (R4 12) (L1 64)
                         (par (R2 10)
                          (seq (C2 0.0001)
                           (par (R3 120) (C1 0.0001)))))))

(defun par (&rest op-list)
  (if (cdr op-list)
      (apply 'par
             (/ (* (car op-list)
                   (cadr op-list))
                (+ (car op-list)
                   (cadr op-list)))
             (cddr op-list))
      (car op-list)))

(defun seq (&rest op-list)
  (if (cdr op-list)
      (apply 'seq
              (+ (car op-list)
                 (cadr op-list))
              (cddr op-list))
      (car op-list)))

(defun calculation-circuit (circuit w)
  (labels ((traverse (circ)
             (cond
               ((null circ) nil)
               ((member (car circ) '(c1 c2))
                (complex (/ -1 (* w (cadr circ))) 0))
               ((eq (car circ) 'l1)
                (complex 0 (* w (cadr circ))))
               ((member (car circ) '(r1 r2 r3 r4))
                (cadr circ))
               ((symbolp (car circ))
                (cons (car circ) (traverse (cdr circ))))
               ((listp (car circ))
                (cons (traverse (car circ)) (traverse (cdr circ)))))) )
    
    (eval (print (traverse circuit)))))


(defun main (&optional (w '(0.01 0.1 10 100)))
   (when w
     (cons `((w ,(car w)) ,(calculation-circuit circuit (car w)))
           (main (cdr w)))))

;;; testing 
;;(main)
;; (((W 0.01) #C(19.187935 0.4867423))
;;  ((W 0.1) #C(19.36767 4.8607097))
;;  ((W 10) #C(141.18301 32.78507))
;;  ((W 100) #C(149.9055 3.5130832)))

