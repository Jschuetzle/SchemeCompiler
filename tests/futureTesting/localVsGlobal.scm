(define store list<real> '(6.5 -9.14))

(define list_addition
    (lambda real ()
        (let ((x (car store))
              (y (car (cdr store))) )
           (+ x y))))

(list_addition)  ; should return -2.64
(+ x y)  ; should produce unknown identifier error



