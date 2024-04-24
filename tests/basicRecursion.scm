(define sum 
    (lambda int (int n) 
        (if (eq? n 0)
            0
            (+ n (sum (- n 1))))))

(sum 8)  ; should return 36
