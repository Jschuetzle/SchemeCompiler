(define fib
    (lambda int (int n)
        (if (eq? 1 n)
            1
            (+ (fib (- n 1))
               (fib (- n 2))))))
