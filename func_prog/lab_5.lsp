
;; 4. Написати функцію стиснення списку F4 (l4), яка заміщає сукупність
;; послідовно розташованих однакових елементів списку двоелементним списком
;; виду (кількість_повторень елемент)
;; Наприклад:
;; >(F4 '(a a a a b c c a a d e e e e))
;; ((4 A) (1 B) (2 C) (2 A) (1 D) (4 E))

(defun f4 (lst &optional elem (count 0))
  (cond
    ((null lst)
     (when elem
       `((,count ,elem)))) 
    (elem
     (if (eq elem (car lst))
         (f4 (cdr lst) elem (1+ count))
         (cons `(,count ,elem) (f4 lst))))
    (t (f4 (cdr lst) (car lst) 1))))

(f4 '(a a a a b c c a a d e e e e)) ; ==> ((4 A) (1 B) (2 C) (2 A) (1 D) (4 E))


