ef: (define (f l)
            18: (let ((_var0 1: (null? 0: l)))
                     17: (if 2: _var0
                             3: l
                             16: (let ((_var1 5: (car 4: l)))
                                      15: (let ((_var2 7: (f 6: _var1)))
                                               14: (let ((_var4 8: 2))
                                                        13: (let ((_var3 11: (cons 9: _var2
                                                                                   10: _var4)))
                                                                 12: _var3)))))))

24: (let ((_var6 19: '()))
         23: (let ((_var7 21: (f 20: _var6)))
                  22: _var7))