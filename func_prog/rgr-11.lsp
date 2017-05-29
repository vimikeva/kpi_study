;;; rgr-11

(defun func (n)
  (labels ((%func (n)
             (cond
               ((or (= n 1) (= n 11)) 1)
               ((and (> n 1) (< n 11))
                (+ (sin (%func (1- n)))
                   (* n 1/2
                      (cos (%func (1- n))))))
               ((and (> n 11) (< n 21))
                (+ (* 2 (cos (%func (1- n))))
                   (* 2 n))))))

    (if (and (> n 0) (< n 21))
        (format t "F(~A) = ~A~%" n (%func n))
        (format t "Error! n = ~A isn't valid~%" n))))


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
;; F(2) = 1.3817732
;; F(6) = 2.3940182
;; F(9) = -3.4373622
;; F(10) = -4.4914145
;; F(11) = 1
;; F(15) = 31.173874
;; F(20) = 39.13474
;; Error! n = 21 isn't valid
;; Error! n = 9999999 isn't valid
