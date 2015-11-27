(define (not x)
        (if x
            (let ((_var0 #f))
                 _var0)
            (let ((_var1 #t))
                 _var1)))

(define (listn n)
        (let ((_var2 0))
             (let ((_var3 (= _var2
                             n)))
                  (let ((_var4 (not _var3)))
                       (if _var4
                           (let ((_var5 1))
                                (let ((_var6 (- n
                                                _var5)))
                                     (let ((_var7 (listn _var6)))
                                          (let ((_var8 (cons n
                                                             _var7)))
                                               _var8))))
                           (let ((_var9 '()))
                                _var9))))))

(define (l18l)
        (let ((_var10 18))
             (let ((_var11 (listn _var10)))
                  _var11)))

(define (l12l)
        (let ((_var12 12))
             (let ((_var13 (listn _var12)))
                  _var13)))

(define (l6l)
        (let ((_var14 2))
             (let ((_var15 (listn _var14)))
                  _var15)))

(define (mas x y z)
        (let ((_var16 (shorterp y
                                x)))
             (let ((_var17 (not _var16)))
                  (if _var17
                      z
                      (let ((_var18 (cdr x)))
                           (let ((_var19 (mas _var18
                                              y
                                              z)))
                                (let ((_var21 (cdr y)))
                                     (let ((_var22 (mas _var21
                                                        z
                                                        x)))
                                          (let ((_var24 (cdr z)))
                                               (let ((_var25 (mas _var24
                                                                  x
                                                                  y)))
                                                    (let ((_var20 (mas _var19
                                                                       _var22
                                                                       _var25)))
                                                         _var20)))))))))))

(define (shorterp x y)
        (let ((_var27 (null? y)))
             (if _var27
                 (let ((_var28 #f))
                      _var28)
                 (let ((_var29 (null? x)))
                      (if _var29
                          (let ((_var30 #t))
                               _var30)
                          (let ((_var31 (cdr x)))
                               (let ((_var33 (cdr y)))
                                    (let ((_var32 (shorterp _var31
                                                            _var33)))
                                         _var32))))))))

(let ((_var35 18))
     (let ((_var37 '()))
          (let ((_var36 (cons _var35
                              _var37)))
               (let ((_var40 12))
                    (let ((_var42 '()))
                         (let ((_var41 (cons _var40
                                             _var42)))
                              (let ((_var45 6))
                                   (let ((_var47 '()))
                                        (let ((_var46 (cons _var45
                                                            _var47)))
                                             (let ((_var39 (mas _var36
                                                                _var41
                                                                _var46)))
                                                  _var39))))))))))