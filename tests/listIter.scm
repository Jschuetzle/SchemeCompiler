(define example list<int> '(6 7 1 2))

(define evens
    (lambda list<int> (list<int> l)
        (cond
            ((null? l)                '())
            ((eq? (% (car l) 2) 0)    (cons (car l) (evens (cdr l))))
            (else                     (evens (cdr l))))))

(evens example)  ; should return '(6 2)
