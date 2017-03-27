;;; rgr

(defun func (n)
  (labels ((%func (n)
             (cond
               ((or (= n 1) (= n 10)) 1)
               ((and (> n 1) (< n 10))
                (+ (* 2 (%func (1- n)))
                   (cos (- (expt short-float-epsilon n) n))))
               ((and (> n 10) (<= n 20))
                (- (sin n)
                   (* 2 (cos (%func (1- n)))))))))

    (if (and (> n 0) (< n 21))
        (format t "~%F(~A) = ~A~%" n (%func n))
        (format t "~%Error! n = ~A isn't valid~%" n))))


;; testing
(defun test ()
  (func 0)
  (func 1)
  (func 2)
  (func 6)
  (func 9)
  (func 10)
  (func 11)
  (func 15)
  (func 20)
  (func 21)
  (func 9999999))

;; (test)
;;; rgr

(defun func (n)
  (labels ((%func (n)
             (cond
               ((or (= n 1) (= n 10)) 1)
               ((and (> n 1) (< n 10))
                (+ (* 2 (%func (1- n)))
                   (cos (- (expt short-float-epsilon n) n))))
               ((and (> n 10) (< n 20))
                (- (sin n)
                   (* 2 (cos (%func (1- n)))))))))

    (if (and (> n 0) (< n 21))
        (format t "~%F(~A) = ~A~%" n (%func n))
        (format t "~%Error! n = ~A isn't valid~%" n))))


;; testing
(defun test ()
  (func 0)
  (func 1)
  (func 2)
  (func 6)
  (func 9)
  (func 10)
  (func 11)
  (func 15)
  (func 20)
  (func 21)
  (func 9999999))

;; (test)
;; Error! n = 0 isn't valid
;; F(1) = 1
;; F(2) = 1.5838531
;; F(6) = 16.334631
;; F(9) = 132.49052
;; F(10) = 1
;; F(11) = -2.0805948
;; F(15) = -0.9650235
;; F(20) = -1.0384262
;; Error! n = 21 isn't valid
;; Error! n = 9999999 isn't valid
