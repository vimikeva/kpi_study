;; comp system
;; lab_2
;; single-CPU scheduler
;; (c) Mykhailo Vitovshchyk (vimikeva, 13th)


(defparameter *processors* '((0 3) ; 0 proc
                             (0 2) ; 1 proc
                             (0 8) ; 2 proc
                             (0 10) ; 3 proc
                             (0 1))  ; 4 proc
                          ;; (free difficult)
  )

(defparameter *tasks* '(((0) 100)
                        ((1) 150)
                        ((3) 140)
                        ((4) 180)
                        ((1) 200)
                        ((2) 130)
                        ((4) 75)
                        ; ... ... ...
                        ))

(defparameter *likelihood* 30)


(defun sort-tasks (lst)
  (labels ((%exch (lst)
             (if (and lst (cdr lst))
               (if (< (second (car lst)) (second (cadr lst)))
                   (cons (cadr lst)
                         (%exch (cons (car lst) (cddr lst))))
                   (cons (car lst)
                         (%exch (cdr lst))))
               lst))

           (%check (lst)
             (let ((res-lst (%exch lst)))
               (if (equal res-lst lst)
                   res-lst
                   (%check res-lst)))))
    (%check lst)))


(defun processor (&optional task-tacts procs)
  "stream processors"
  (mapcar (lambda (proc)
            (when (>= proc (length *processors*))
              (error "Processor ~A don't found" proc)))
          procs)
  ;; 1 mc
  (setf *processors*
        (mapcar (lambda (proc)
                  `(,(if (< 0 (- (car proc) (cadr proc)))
                         (- (car proc) (cadr proc))
                         0)
                    ,(cadr proc)))
                *processors*))
  
  ;; set 'task-tact' tacts in 'proc' processor
  (when procs
    (maplist (lambda (proc task-tact)
               (when (= 0 (car (nth (car proc) *processors*)))
                 (setf (nth (car proc) *processors*)
                       `(,(car task-tact) ,(second (nth (car proc) *processors*))))))
             procs
             task-tacts)))


(defparameter *count-operations* 0)
(defparameter *max-count-operations* 0)

(defun print-processors ()
  (dolist (proc *processors*)
    (incf *max-count-operations*)
    (unless (= 0 (car proc))
      (incf *count-operations*))
    (format t "  ~A" (car proc)))
  (format t "~%"))


(defun task-manager (&optional (tasks *tasks*) run-task)
  "tasks and processors manager"
  (when (or tasks
            (reduce (lambda (acc proc)
                      (cond
                        (acc t)
                        ((/= 0 (car proc)) t)))
                    *processors*
                    :initial-value nil))
    (sleep 0.2)
    (when (and tasks
               (<= (1+ (random 100 (make-random-state t))) *likelihood*))
      (push (car tasks) run-task)
      (setf tasks (cdr tasks)))

    (let (proc-lst tact-lst)
      (if run-task
          (progn
            (setf run-task
                  (remove-if (lambda (task)
                               (let ((proc
                                       (reduce (lambda (acc proc)
                                                 (cond
                                                   ((null acc)
                                                    (if (= 0 (car (nth proc *processors*)))
                                                        proc
                                                        acc))
                                                   ((> (second (nth proc *processors*))
                                                       (second (nth acc *processors*)))
                                                    proc)
                                                   (t acc)))
                                               (car task)
                                               :initial-value nil)))
                                 (if proc
                                     (progn
                                       (push (second task) tact-lst)
                                       (push proc proc-lst)
                                       t)
                                     nil)))
                             (sort-tasks run-task)))
            (if proc-lst
                (processor tact-lst proc-lst)
                (processor))
            (print-processors)
            (task-manager tasks run-task))
          (progn
            (processor)
            (print-processors)
            (task-manager tasks run-task))) )))


(defun proc-task (name)
  (let ((count -1)
        tact)
    (format t "List ~AS:~%" (symbol-name name))
    (mapcar (lambda (elem)
              (format t "  ~A ~A: ~A~%"
                      (symbol-name name)
                      (setf count (1+ count))
                      (if (eq :task name)
                          (format nil "~A ~A" (second elem) (car elem))
                          (second elem))))
            (if (eq :task name)
                *tasks*
                *processors*))

    (format t "1. Add~%2. Remove~%0. Back~%~%--> ")
    (setq count (read-line))
    (cond
      ((string= count "1")
       (if (eq name :task)
           (progn
             (format t "Enter count tacts: ")
             (setq tact (parse-integer (read-line)))
             (format t "Enter processors (through enter, enpty line = end):~%--> ")
             (setq *tasks*
                   (cons (list
                          (loop while (string/= (setq count (read-line)) "")
                                do (format t "--> ")
                                collect (parse-integer count))
                          tact)
                         *tasks*)))
           (progn
             (format t "Processor type complexity: ")
             (setq *processors*
                   (cons `(0 ,(parse-integer (read-line)))
                         *processors*)))))

      ((string= count "2")
       (format t "Enter number ~A: " (symbol-name name))
       (let ((count -1)
             (number (parse-integer (read-line))))
         (if (eq name :task)
             (setq *tasks*
                   (remove-if (lambda (elem)
                                (declare (ignorable elem))
                                (if (= number (setq count (1+ count)))
                                    t nil))
                              *tasks*))
             (setq *processors*
                   (remove-if (lambda (elem)
                                (declare (ignorable elem))
                                (if (= number (setq count (1+ count)))
                                    t nil))
                              *processors*)))))
      
      ((string/= count "0")
       (format t "Error!")
         (read-char)))
    (if (string/= count "0")
        (proc-task name))))


(defun run ()
  "main menu program for change processors and tasks"
  (let ((lst-menu '("Menu:"
                    "  1. The processors"
                    "  2. The likelihood of create tasks"
                    "  3. The tasks"
                    "  4. Run"
                    "  0. Exit"))
        count likelihood)
    (format t "~A~%" (first lst-menu))
    (format t "~A (~A)~%" (second lst-menu) (length *processors*))
    (format t "~A (~A%)~%" (third lst-menu) *likelihood*)
    (format t "~A (~A)~%" (fourth lst-menu) (length *tasks*))
    (format t "~A~%" (fifth lst-menu))
    (format t "~A~%~%--> " (sixth lst-menu))

    (setq count (read-line))
    (cond
      ((string= count "1")
       (proc-task :proc))
      ((string= count "2")
       (format t "Enter new likelihood (0-100%): ")
       (setq likelihood (parse-integer (read-line)))
       (if (and (< 0 likelihood)
                (>= 100 likelihood))
           (setf *likelihood* likelihood)
           (progn
             (format t "Don't correct likelihood!~%")
             (read-char))))
      ((string= count "3")
       (proc-task :task))
      ((string= count "4")
       (setf *count-operations* 0)
       (setf *max-count-operations* 0)
       (mapcar (lambda (elem)
                 (format t "  ~A" elem))
               (loop for i from 0 to (1- (length *processors*))
                     collect i))
       (format t "~%")
       (task-manager)
       (format t "~%count-op: ~A~%max-count-op: ~A~%KPD: ~3$%~%"
               *count-operations*
               *max-count-operations*
               (/ *count-operations* *max-count-operations* 0.01))
       (read-char))
      ((string/= count "0")
       (format t "Error!~%")))
    (unless (string= count "0")
      (run)) ))
