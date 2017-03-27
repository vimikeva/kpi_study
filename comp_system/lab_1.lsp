;; comp system
;; lab_1
;; fifo
;; (c) Mykhailo Vitovshchyk (vimikeva, 13th)


(defparameter *processors* '((0 3) ; 0 proc
                             (0 2) ; 1 proc
                             (0 8) ; 2 proc
                             (0 10) ; 3 proc
                             (0 1))  ; 4 proc
                          ;; (free difficult)
  )

(defparameter *tasks* '(((0 1) 100)
                        ((1) 150)
                        ((3) 140)
                        ((0 4 2) 180)
                        ((1 2 3) 200)
                        ((2) 130)
                        ((0 2 3 4) 75)
                        ; ... ... ...
                        ))


(defparameter *likelihood* 30)


(defun processor (&optional task-tact procs)
  "stream processors"
  (dolist (proc procs)
    (when (>= proc (length *processors*))
      (error "Processor ~A don't found" proc)))
  ;; 1 mc
  (setf *processors*
        (mapcar (lambda (proc)
                  `(,(if (< 0 (- (car proc) (cadr proc)))
                         (- (car proc) (cadr proc))
                         0)
                    ,(cadr proc)))
                *processors*))

  ;; if task is that it is work.
  (when task-tact
    (let* ((num-proc
             (reduce (lambda (c-num num)
                       (cond
                         ((not (= 0 (car (nth num *processors*)))) c-num)
                         ((= c-num -1) num)
                         ((> (second (nth num *processors*))
                             (second (nth c-num *processors*)))
                          num)
                         (t c-num)))
                     procs
                     :initial-value -1)))
      (if (= num-proc -1)
          :wait
          (setf (car (nth num-proc *processors*))
                (- task-tact (second (nth num-proc *processors*))))))) )

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
    (if run-task
        (if (eq :wait (processor (second run-task) (car run-task)))
            (progn
              (print-processors)
              (task-manager tasks run-task))
            (progn
              (print-processors)
              (task-manager tasks)))
        (let ((rand (1+ (random 100 (make-random-state t)))))
          (if (<= rand *likelihood*)
              (progn
                (if (eq :wait (processor (second (car tasks)) (caar tasks)))
                    (progn
                      (print-processors)
                      (task-manager (cdr tasks) (car tasks)))
                    (progn
                      (print-processors)
                      (task-manager (cdr tasks)))))
              (progn
                (processor)
                (print-processors)
                (task-manager tasks))))) ))


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
