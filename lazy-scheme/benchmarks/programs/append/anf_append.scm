esq: (define (sq x)
             4: (let ((_var0 2: (* 0: x
                                   1: x)))
                     3: _var0))

edouble: (define (double x)
                 9: (let ((_var1 7: (+ 5: x
                                       6: x)))
                         8: _var1))

eappend: (define (append ax ay)
                 30: (let ((_var2 11: (null? 10: ax)))
                          29: (if 12: _var2
                                  13: ay
                                  28: (let ((_var3 15: (car 14: ax)))
                                           27: (let ((_var5 17: (cdr 16: ax)))
                                                    26: (let ((_var6 20: (append 18: _var5
                                                                                 19: ay)))
                                                             25: (let ((_var4 23: (cons 21: _var3
                                                                                        22: _var6)))
                                                                      24: _var4)))))))

63: (let ((_var8 31: 1))
         62: (let ((_var10 32: 2))
                  61: (let ((_var11 34: (double 33: _var10)))
                           60: (let ((_var12 36: (sq 35: _var11)))
                                    59: (let ((_var14 37: '()))
                                             58: (let ((_var13 40: (cons 38: _var12
                                                                         39: _var14)))
                                                      57: (let ((_var9 43: (cons 41: _var8
                                                                                 42: _var13)))
                                                               56: (let ((_var18 44: 3))
                                                                        55: (let ((_var20 45: '()))
                                                                                 54: (let ((_var19 48: (cons 46: _var18
                                                                                                             47: _var20)))
                                                                                          53: (let ((_var17 51: (append 49: _var9
                                                                                                                        50: _var19)))
                                                                                                   52: _var17)))))))))))