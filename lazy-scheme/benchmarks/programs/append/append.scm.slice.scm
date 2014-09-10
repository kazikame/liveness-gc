(define (sq x)
        (let ((_var0 (* x
                        x)))
             _var0))

(define (double x)
        (let ((_var1 (+ x
                        x)))
             _var1))

(define (append ax ay)
        (let ((_var2 (null? ax)))
             (if _var2
                 ay
                 (let ((_var3 (car ax)))
                      (let ((_var5 (cdr ax)))
                           (let ((_var6 (append _var5
                                                ay)))
                                (let ((_var4 (cons _var3
                                                   _var6)))
                                     _var4)))))))

(let ((_var8 1))
     (let ((_var10 2))
          (let ((_var11 (double _var10)))
               (let ((_var12 (sq _var11)))
                    (let ((_var14 '()))
                         (let ((_var13 (cons _var12
                                             _var14)))
                              (let ((_var9 (cons _var8
                                                 _var13)))
                                   (let ((_var18 3))
                                        (let ((_var20 '()))
                                             (let ((_var19 (cons _var18
                                                                 _var20)))
                                                  (let ((_var17 (append _var9
                                                                        _var19)))
                                                       _var17)))))))))))