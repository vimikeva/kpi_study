;; lab_7 var_1 (phonebook)

(defstruct phonebook
  pib
  n-phone
  address)

(defparameter *phonebook* nil)


(defun list-all (&optional (phb *phonebook*))
  (format t "---------------------------------------~%")
  (when phb
    (progn
      (format t "Name-Surname: ~A~%N-phone: ~A~%Address: ~A~%"
              (phonebook-pib (car phb))
              (phonebook-n-phone (car phb))
              (phonebook-address (car phb)))
      (list-all (cdr phb)))))


(defun add-new-contact (contact &optional (phb *phonebook*))
  (if phb
      (unless (and (string-equal (first contact) (phonebook-pib (car phb)))
                   (string-equal (second contact) (phonebook-n-phone (car phb)))
                   (string-equal (third contact) (phonebook-address (car phb))))
        (add-new-contact contact (cdr phb)))
      (setq *phonebook*
            (cons (make-phonebook :pib (first contact)
                                    :n-phone (second contact)
                                    :address (third contact))
                   *phonebook*))))

(defun delete-contact (contact &optional (phb *phonebook*))
  (let (res)
    (setq *phonebook*
          (remove-if #'(lambda (element)
                       (and (string-equal (first contact) (phonebook-pib element))
                            (string-equal (second contact) (phonebook-n-phone element))
                            (string-equal (third contact) (phonebook-address element))
                            (setq res t)))
                     phb))
    res))


(defun list-contact-by-name-mask (mask &optional (phb *phonebook*))
  (labels ((%equal (name mask)
             ;;(format t "name: ~A  mask: ~A" name mask)
             (cond
               ((null mask) t)
               ((and (char-equal #\* (car mask))
                     (null (cdr mask))) t)
               ((null name) nil)
               ((char-equal #\* (car mask))
                (if (and (char-equal (second mask) (first name))
                         (%equal (cdr name) (cddr mask)))
                    t
                    (%equal (cdr name) mask)))
               ((char-equal (car name) (car mask))
                (%equal (cdr name) (cdr mask)))))
           (%print-for-mask (phbook mask)
             (if phbook
                 (if (%equal (coerce (phonebook-pib (car phbook)) 'list)
                             (coerce mask 'list))
                     (progn 
                       (format t "---------------------------------------~%")
                       (format t "Name-Surname: ~A~%N-phone: ~A~%Address: ~A~%"
                               (phonebook-pib (car phbook))
                               (phonebook-n-phone (car phbook))
                               (phonebook-address (car phbook)))
                       (%print-for-mask (cdr phbook) mask))
                     (%print-for-mask (cdr phbook) mask))
                 (format t "---------------------------------------~%"))) )
    (%print-for-mask phb mask)))

(defun find-name-or-phone (name-phone &optional (el-find :name) (phb *phonebook*))
  (cond
    ((null phb)
     (format t "---------------------------------------~%"))
    ((string-equal name-phone (if (eq el-find :name)
                                  (phonebook-pib (car phb))
                                  (phonebook-n-phone (car phb))))
     (format t "---------------------------------------~%")
     (format t "Name-Surname: ~A~%N-phone: ~A~%Address: ~A~%"
             (phonebook-pib (car phb))
             (phonebook-n-phone (car phb))
             (phonebook-address (car phb)))
     (find-name-or-phone name-phone el-find (cdr phb)))
    (t (find-name-or-phone name-phone el-find (cdr phb)))))


(defun menu ()
  (let ((menu-elements '("1. List All"
                         "2. Add new contact"
                         "3. Delete contact"
                         "4. List contacts by name mask"
                         "5. Find by name"
                         "6. Find by phone"
                         "0. Exit") ))
    (format t "~%~%")
    (mapcar (lambda (elem)
              (format t "~A~%" elem))
          menu-elements)
    (format t "\$> "))
  (let ((choice (read-line)))
    (cond
      ((string-equal choice "0")
       (format t "The End.~%"))
      ((string-equal choice "1")
       (list-all)
       (read-char))
      ((string-equal choice "2")
       (let (name-surname n-phone address)
         (format t "Enter Name Surname: ")
         (setq name-surname (read-line))
         (format t "Enter number phone: ")
         (setq n-phone (read-line))
         (format t "Enter address:")
         (setq address (read-line))
         (if (add-new-contact `(,name-surname ,n-phone ,address))
             (format t "~%==| Added contact")
             (format t "~%==| Don't added contact"))
         (read-char)))
      ((string-equal choice "3")
       (let (name-surname n-phone address)
         (format t "Enter Name Surname: ")
         (setq name-surname (read-line))
         (format t "Enter number phone: ")
         (setq n-phone (read-line))
         (format t "Enter address:")
         (setq address (read-line))
         (if (delete-contact `(,name-surname ,n-phone ,address))
             (format t "~%==| Deleted contact")
             (format t "~%==| Don't deleted contact"))
         (read-char)) )
      ((string-equal choice "4")
       (format t "Enter mask: ")
       (list-contact-by-name-mask (read-line))
       (read-char))
      ((string-equal choice "5")
       (let (name)
         (format t "name: ")
         (setq name (read-line))
         (find-name-or-phone name :name))
         (read-char))
      ((string-equal choice "6")
       (let (phone)
         (format t "phone: ")
         (setq phone (read-line))
         (find-name-or-phone phone :phone))
         (read-char))
      (t (format t "Error! Not exist label.~%")))
    (when (string-not-equal choice "0")
      (menu))))
