p#0: (define (sq x)
             p#1: (let ((_var0 p#2: (* p#3: x
                                       p#4: x)))
                       p#5: _var0))

p#6: (define (double x)
             p#7: (let ((_var1 p#8: (+ p#9: x
                                       p#10: x)))
                       p#11: _var1))

p#12: (define (append ax ay)
              p#13: (let ((_var2 p#14: (null? p#15: ax)))
                         p#16: (if p#17: _var2
                                   p#18: ay
                                   p#19: (let ((_var3 p#20: (car p#21: ax)))
                                              p#22: (let ((_var5 p#23: (cdr p#24: ax)))
                                                         p#25: (let ((_var6 p#26: (append p#27: _var5
                                                                                          p#28: ay)))
                                                                    p#29: (let ((_var4 p#30: (cons p#31: _var3
                                                                                                   p#32: _var6)))
                                                                               p#33: _var4)))))))

p#34: (let ((_var8 p#35: 1))
           p#36: (let ((_var10 p#37: 2))
                      p#38: (let ((_var11 p#39: (double p#40: _var10)))
                                 p#41: (let ((_var12 p#42: (sq p#43: _var11)))
                                            p#44: (let ((_var14 p#45: '()))
                                                       p#46: (let ((_var13 p#47: (cons p#48: _var12
                                                                                       p#49: _var14)))
                                                                  p#50: (let ((_var9 p#51: (cons p#52: _var8
                                                                                                 p#53: _var13)))
                                                                             p#54: (let ((_var18 p#55: 3))
                                                                                        p#56: (let ((_var20 p#57: '()))
                                                                                                   p#58: (let ((_var19 p#59: (cons p#60: _var18
                                                                                                                                   p#61: _var20)))
                                                                                                              p#62: (let ((_var17 p#63: (append p#64: _var9
                                                                                                                                                p#65: _var19)))
                                                                                                                         p#66: _var17)))))))))))