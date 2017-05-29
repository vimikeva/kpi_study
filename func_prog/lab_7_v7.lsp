;; lab 7
;; var 7(2)

(defstruct films
  name
  producer
  year
  rating)

(defparameter *films* nil) ;; defparameter => defvar

(defun list-all (&optional (flm *films*) (number 1))
  (format t "~%")
  (unless *films*
    (format t "List is empty!~%"))
  (when flm
    (format t "~A. Name: ~A  Producer: ~A  Year: ~A  Rating: ~A~%"
            number
            (films-name (car flm))
            (films-producer (car flm))
            (films-year (car flm))
            (films-rating (car flm)))
    (list-all (cdr flm) (+ number 1))))

(defun add-new (film &optional (flm *films*))
  (if flm
    (unless (and (string= (films-name film) (films-name (car flm)))
                 (string= (films-producer film) (films-producer (car flm)))
                 (string= (films-year film) (films-year (car flm))))
      (add-new film (cdr flm)))
    (setq *films* (cons film *films*))))

(defun del (number &optional (flm *films*))
  (when flm
    (if (= number 1)
        (cdr flm)
        (cons (car flm) (del (- number 1) (cdr flm))))))

(defun delete-elem ()
  (if *films*
      (progn
        (list-all)
        (format t "~%Select number of element: ")
        (let (number)
          (setq number (read-line))
          (setq *films* (del (parse-integer number)))))
      (format t "List is empty!~%")))


(defun find-by-attr (nm fn &optional (flm *films*))
  (when flm
    (when (string= nm (funcall fn (car flm)))
      (format t "~%Name: ~A  Producer: ~A  Year: ~A  Rating: ~A"
              (films-name (car flm))
              (films-producer (car flm))
              (films-year (car flm))
              (films-rating (car flm))))
    (find-by-attr nm fn (cdr flm))))

(defun find-by (&key name producer rating)
  (cond
    (name (find-by-attr name 'films-name))
    (producer (find-by-attr producer 'films-producer))
    (rating (find-by-attr rating 'films-rating))))

(defun mask-search (mask name)
  (cond
    ((and (or (eq (length mask) 0)
              (and (eq (length mask) 1)
                   (char= (char mask 0) #\*)))
          (eq (length name) 0)) t)
     ((eq (length mask) 0) nil)
     ((eq (length name) 0) nil)
     ((char= (char mask 0) (char name 0))
      (mask-search (subseq mask 1) (subseq name 1)))
     ((and (char= (char mask 0) #\*)
           (eql (1- (length mask))
                (length name)))
      (mask-search (subseq mask 1) name))
     ((char= (char mask 0) #\*)
      (mask-search mask (subseq name 1)))
     (t nil)))

(defun find-by-mask (mask &optional (flm *films*))
  (when flm
    (when (mask-search mask (films-name (car flm)))
      (format t "~%Name: ~A  Producer: ~A  Year: ~A  Rating: ~A"
              (films-name (car flm))
              (films-producer (car flm))
              (films-year (car flm))
              (films-rating (car flm))))
    (find-by-mask mask (cdr flm))))

  
(defun menu ()
  (let ((elem-menu '(" 1. List all"
                     " 2. Add new elem"
                     " 3. Delete elem"
                     " 4. Find by name films"
                     " 5. Find by name producer"
                     " 6. Find by rating"
                     " 7. List film by name mask"
                     " 0. Exit"))
        select)
    (format t "~%~%Menu:~%")
    (mapc (lambda (elem)
            (format t "~A~%" elem))
          elem-menu)
    (format t "Select: ")
    (setq select (read-line))
    
    (cond
      ((string= select "1")
       (list-all)
       (menu))
      ((string= select "2")
       (let (name producer year rating)
         (format t "Enter name: ")
         (setq name (read-line))
         (format t "Enter producer: ")
         (setq producer (read-line))
         (format t "Enter year: ")
         (setq year (read-line))
         (format t "Enter rating: ")
         (setq rating (read-line))
         (add-new (make-films :name name
                              :producer producer
                              :year year
                              :rating rating)))
       (menu))
      ((string= select "3")
       (delete-elem)
       (menu))

      ((string= select "4")
       (format t "Enter name film: ")
       (let ((name (read-line)))
         (find-by :name name))
       (menu))

      ((string= select "5")
       (format t "Enter name producer: ")
       (let ((producer (read-line)))
         (find-by :producer producer))
       (menu))

      ((string= select "6")
       (format t "Enter name rating: ")
       (let ((rating (read-line)))
         (find-by :rating rating))
       (menu))

      ((string= select "7")
       (format t "Enter mask: ")
       (let ((mask (read-line)))
         (find-by-mask mask))
       (menu))
      
      ((string= select "0"))
      (t
       (format t "Error input")
       (menu)))
      
  ))
