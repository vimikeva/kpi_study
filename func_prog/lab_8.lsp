;; lab_8 travers-graph

(defun depth-first (graph beg end)
  (let (res-list)
    (labels ((%df-element (node)
               (cond
                 ((reduce (lambda (acc nd)
                            (cond
                              ((and (eq (car nd) (caar node))
                                    (eq (caar node) end))
                               acc)
                              ((and (eq (car nd) (caar node))
                                    (not (member (second nd) (car node))))
                               (cons `(,(cons (second nd) (car node))
                                       ,(+ (third nd) (cadr node)))
                                     acc))
                              (t acc)))
                          graph
                          :initial-value nil))
                 (t (list node))))
             
             (%df (nodes)
               (when nodes
                 (let ((way (%df-element (car nodes))))
                   (if (equal (car nodes) (car way))
                       (progn
                         (push (car way) res-list)
                         (%df (cdr nodes)))
                       (%df (append (cdr nodes) way)))))) )
      
      (%df `(((,beg) 0)))
      (reduce (lambda (acc nd)
                (if acc
                    (if (and (eq (caar nd) end)
                             (< (second nd) (second acc)))
                        nd
                        acc)
                    (when (eq (caar nd) end)
                      nd)))
              res-list
              :initial-value nil))))


(defun breadth-first (graph beg end)
  (let ((res-list))
    (labels ((%bf-element (node)
               (cond
                 ((reduce (lambda (acc nd)
                            (if (eq (car nd) (caar node))
                              (cond
                                ((eq (caar node) end) acc)
                                ((not (member (second nd) (car node)))
                                 (cons `(,(cons (second nd) (car node))
                                         ,(+ (third nd) (cadr node))) acc)))
                              acc))
                          graph :initial-value nil))
                 (t (list node))))
           
           (%bf (nodes)
             (when nodes
               (if (equal (car nodes)
                          (car (%bf-element (car nodes))))
                   (progn
                     (push (car (%bf-element (car nodes)))
                           res-list)
                     (%bf (cdr nodes)))
                   (%bf (append (cdr nodes)
                                (%bf-element (car nodes))))))))
      (%bf `(((,beg) 0)))
      (reduce (lambda (acc nd)
                (if (> (second acc) (second nd))
                    nd acc))
              (remove-if (lambda (nd)
                           (unless (eq (caar nd) end) nd))
                         res-list)))))


(defun degree(graph node &optional power)
  (cond
    ((null graph) (length power))
    ((eq (caar graph) node)
     (degree (cdr graph) node (cons (car graph) power)))
    (t (degree (cdr graph) node power))))


(defun nodes-list (graph)
  (labels ((%get-vertex (vertex)
             (when vertex
               (if (member (car vertex) (cdr vertex))
                   (%get-vertex (cdr vertex))
                   (cons (car vertex) (%get-vertex (cdr vertex))))))
           
           (%exch (vertex)
             (if (second vertex)
               (if (< (degree graph (first vertex)) (degree graph (second vertex)))
                   (cons (second vertex) (%exch (cons (first vertex) (cddr vertex))))
                   (cons (car vertex) (%exch (cdr vertex))))
               vertex))
           
           (%check (vertex)
             (let ((new-v (%exch vertex))
                   check)
               (reduce (lambda (acc vert)
                         (when (< (degree graph  acc) (degree graph vert))
                           (setq check t))
                         vert)
                       new-v)
               (if check
                   (%check new-v)
                   new-v)
               
               )))
    (%check
     (%get-vertex (mapcar (lambda (nd) (car nd)) graph)))))


           


;;;testing

(defparameter graph '( (k d 6) (d b 7) (a k 9) (b a 2) (a b 4) (a d 5) (b c 3) (c a 1)))

;; (print (depth-first graph 'a 'd)) ;; ==> ((D A) 5)
;; (print (breadth-first graph 'a 'd)) ;; ==> ((D A) 5)
;; (print (degree graph 'a)) ;; ==> 3
;; (print (nodes-list graph)) ;; ==> (A B K D C) 
