(define (length l)
  (if (null? l) 0
      (+ 1 (length (cdr l)))))

(define (append x y)
  (if (null? x) y
      (cons (car x) (append (cdr x) y))))

(define (f l)
  (if (null? l) 0
      (+ (car l) (g (cdr l)))))

(define (g l)
  (if (null? l) 1
      (f (cdr l))))

(define (make_list n)
  (if (= n 0) '()
      (cons n (make_list (- n 1)))))

(let ((var1 (make_list 5)))
      (let ((var2 (make_list 3)))
        (let ((var3 (append var1 var2)))
          (let ((var4 (make_list 6)))
            (let ((var5 (f (make_list 3))))
              (let ((var6 (cdr (cdr (cdr var4)))))
                (if (null? (cdr var3)) (null? var6)
                    var5)))))))
        

