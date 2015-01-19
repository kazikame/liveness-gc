p#0: (define (append ax ay)
             p#1: (let ((_var0 p#2: (null? p#3: ax)))
                       p#4: (if p#5: _var0
                                p#6: ay
                                p#7: (let ((_var1 p#8: (car p#9: ax)))
                                          p#10: (let ((_var3 p#11: (cdr p#12: ax)))
                                                     p#13: (let ((_var4 p#14: (append p#15: _var3
                                                                                      p#16: ay)))
                                                                p#17: (let ((_var2 p#18: (cons p#19: _var1
                                                                                               p#20: _var4)))
                                                                           p#21: _var2)))))))

p#22: (let ((_var6 p#23: 1))
           p#24: (let ((_var8 p#25: 2))
                      p#26: (let ((_var10 p#27: '()))
                                 p#28: (let ((_var9 p#29: (cons p#30: _var8
                                                                p#31: _var10)))
                                            p#32: (let ((_var7 p#33: (cons p#34: _var6
                                                                           p#35: _var9)))
                                                       p#36: (let ((_var14 p#37: 3))
                                                                  p#38: (let ((_var16 p#39: '()))
                                                                             p#40: (let ((_var15 p#41: (cons p#42: _var14
                                                                                                             p#43: _var16)))
                                                                                        p#44: (let ((_var13 p#45: (append p#46: _var7
                                                                                                                          p#47: _var15)))
                                                                                                   p#48: _var13)))))))))

