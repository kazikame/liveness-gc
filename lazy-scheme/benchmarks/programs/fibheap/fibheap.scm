(define (divide1 a b n op)
        (let ((_var0 (< a
                        b)))
             (if _var0
                 (let ((_var1 0))
                      (let ((_var2 (eq? op
                                        _var1)))
                           (if _var2
                               n
                               a)))
                 (let ((_var3 (- a
                                 b)))
                      (let ((_var5 1))
                           (let ((_var6 (+ _var5
                                           n)))
                                (let ((_var4 (divide1 _var3
                                                      b
                                                      _var6
                                                      op)))
                                     _var4)))))))

(define (modr a b)
        (let ((_var8 0))
             (let ((_var10 1))
                  (let ((_var9 (divide1 a
                                        b
                                        _var8
                                        _var10)))
                       _var9))))

(define (append ax ay)
        (let ((_var12 (null? ax)))
             (if _var12
                 ay
                 (let ((_var13 (car ax)))
                      (let ((_var15 (cdr ax)))
                           (let ((_var16 (append _var15
                                                 ay)))
                                (let ((_var14 (cons _var13
                                                    _var16)))
                                     _var14)))))))

(define (insert x heap)
        (let ((_var18 0))
             (let ((_var20 '()))
                  (let ((_var19 (cons _var18
                                      _var20)))
                       (let ((newnode (cons x
                                            _var19)))
                            (let ((_var23 (null? heap)))
                                 (if _var23
                                     (let ((_var24 '()))
                                          (let ((_var25 (cons newnode
                                                              _var24)))
                                               _var25))
                                     (let ((_var26 (cons newnode
                                                         heap)))
                                          _var26))))))))

(define (updateMin curmin heap)
        (let ((_var27 (car curmin)))
             (let ((_var28 (car _var27)))
                  (let ((_var30 (car heap)))
                       (let ((_var31 (car _var30)))
                            (let ((_var29 (< _var28
                                             _var31)))
                                 (if _var29
                                     curmin
                                     heap)))))))

(define (addChild root node)
        (let ((_var33 (car root)))
             (let ((_var34 (cdr _var33)))
                  (let ((ptchild (cdr _var34)))
                       (let ((_var36 (car node)))
                            (let ((_var38 '()))
                                 (let ((_var37 (cons _var36
                                                     _var38)))
                                      (let ((_var40 (append ptchild
                                                            _var37)))
                                           _var40))))))))

(define (updateArray nchild root array)
        (let ((_var41 (null? array)))
             (if _var41
                 (let ((_var42 (car root)))
                      (let ((_var44 '()))
                           (let ((_var43 (cons _var42
                                               _var44)))
                                _var43)))
                 (let ((_var46 (car array)))
                      (let ((_var47 (cdr _var46)))
                           (let ((achild (car _var47)))
                                (let ((_var49 (eq? nchild
                                                   achild)))
                                     (if _var49
                                         (let ((_var50 (car root)))
                                              (let ((_var51 (car _var50)))
                                                   (let ((_var53 (car array)))
                                                        (let ((_var54 (car _var53)))
                                                             (let ((_var52 (< _var51
                                                                              _var54)))
                                                                  (if _var52
                                                                      (let ((ptchild (addChild root
                                                                                               array)))
                                                                           (let ((_var56 (car root)))
                                                                                (let ((_var57 (car _var56)))
                                                                                     (let ((_var59 1))
                                                                                          (let ((_var60 (+ _var59
                                                                                                           nchild)))
                                                                                               (let ((_var61 (cons _var60
                                                                                                                   ptchild)))
                                                                                                    (let ((_var58 (cons _var57
                                                                                                                        _var61)))
                                                                                                         (let ((_var64 (cdr array)))
                                                                                                              (let ((newnode (cons _var58
                                                                                                                                   _var64)))
                                                                                                                   newnode)))))))))
                                                                      (let ((ptchild (addChild array
                                                                                               root)))
                                                                           (let ((_var66 (car array)))
                                                                                (let ((_var67 (car _var66)))
                                                                                     (let ((_var69 1))
                                                                                          (let ((_var70 (+ _var69
                                                                                                           achild)))
                                                                                               (let ((_var71 (cons _var70
                                                                                                                   ptchild)))
                                                                                                    (let ((_var68 (cons _var67
                                                                                                                        _var71)))
                                                                                                         (let ((_var74 (cdr array)))
                                                                                                              (let ((newnode (cons _var68
                                                                                                                                   _var74)))
                                                                                                                   newnode)))))))))))))))
                                         (let ((_var76 (car array)))
                                              (let ((_var78 (cdr array)))
                                                   (let ((_var79 (updateArray nchild
                                                                              root
                                                                              _var78)))
                                                        (let ((_var77 (cons _var76
                                                                            _var79)))
                                                             _var77))))))))))))

(define (getLen root)
        (let ((_var81 (null? root)))
             (if _var81
                 (let ((_var82 0))
                      _var82)
                 (let ((_var83 1))
                      (let ((_var85 (cdr root)))
                           (let ((_var86 (getLen _var85)))
                                (let ((_var84 (+ _var83
                                                 _var86)))
                                     _var84)))))))

(define (calculateM root min)
        (let ((_var88 (null? root)))
             (if _var88
                 min
                 (let ((_var89 (car root)))
                      (let ((m (car _var89)))
                           (let ((_var91 (car min)))
                                (let ((_var92 (car _var91)))
                                     (let ((_var93 (< m
                                                      _var92)))
                                          (if _var93
                                              (let ((_var94 (cdr root)))
                                                   (let ((_var95 (calculateM _var94
                                                                             root)))
                                                        _var95))
                                              (let ((_var96 (cdr root)))
                                                   (let ((_var97 (calculateM _var96
                                                                             min)))
                                                        _var97)))))))))))

(define (calculateMin root)
        (let ((_var98 (cdr root)))
             (let ((_var99 (calculateM _var98
                                       root)))
                  _var99)))

(define (merge2 root array)
        (let ((_var100 (null? root)))
             (if _var100
                 array
                 (let ((_var101 (car root)))
                      (let ((_var102 (cdr _var101)))
                           (let ((nchild (car _var102)))
                                (let ((narray (updateArray nchild
                                                           root
                                                           array)))
                                     (let ((_var104 (cdr root)))
                                          (let ((_var105 (merge2 _var104
                                                                 narray)))
                                               _var105)))))))))

(define (merge1 root curlen)
        (let ((_var106 '()))
             (let ((newroot (merge2 root
                                    _var106)))
                  (let ((lenheap (getLen newroot)))
                       (let ((_var108 (eq? lenheap
                                           curlen)))
                            (if _var108
                                newroot
                                (let ((_var109 (merge1 newroot
                                                       lenheap)))
                                     _var109)))))))

(define (merge root)
        (let ((curlen (getLen root)))
             (let ((_var110 (merge1 root
                                    curlen)))
                  _var110)))

(define (children_to_rootlist node n)
        (let ((_var111 0))
             (let ((_var112 (eq? _var111
                                 n)))
                  (if _var112
                      (let ((_var113 '()))
                           _var113)
                      (let ((_var114 (car node)))
                           (let ((_var116 (cdr node)))
                                (let ((_var118 1))
                                     (let ((_var119 (- n
                                                       _var118)))
                                          (let ((_var117 (children_to_rootlist _var116
                                                                               _var119)))
                                               (let ((_var115 (cons _var114
                                                                    _var117)))
                                                    _var115))))))))))

(define (deleteAndJoinChildren node heap)
        (let ((_var122 (car node)))
             (let ((_var123 (cdr _var122)))
                  (let ((nchildren (car _var123)))
                       (let ((_var125 0))
                            (let ((_var126 (eq? _var125
                                                nchildren)))
                                 (if _var126
                                     (let ((_var127 (cdr heap)))
                                          _var127)
                                     (let ((_var128 (car node)))
                                          (let ((_var129 (cdr _var128)))
                                               (let ((_var130 (cdr _var129)))
                                                    (let ((chlst (children_to_rootlist _var130
                                                                                       nchildren)))
                                                         (let ((_var132 (cdr heap)))
                                                              (let ((_var133 (append chlst
                                                                                     _var132)))
                                                                   _var133)))))))))))))

(define (delete curmin heap)
        (let ((_var134 (null? heap)))
             (if _var134
                 (let ((_var135 '()))
                      _var135)
                 (let ((_var136 (car curmin)))
                      (let ((_var137 (car _var136)))
                           (let ((_var139 (car heap)))
                                (let ((_var140 (car _var139)))
                                     (let ((_var138 (eq? _var137
                                                         _var140)))
                                          (if _var138
                                              (let ((_var142 (deleteAndJoinChildren curmin
                                                                                    heap)))
                                                   _var142)
                                              (let ((_var143 (car heap)))
                                                   (let ((_var145 (cdr heap)))
                                                        (let ((_var146 (delete curmin
                                                                               _var145)))
                                                             (let ((_var144 (cons _var143
                                                                                  _var146)))
                                                                  _var144)))))))))))))

(define (deleteMin curmin heap)
        (let ((step1 (delete curmin
                             heap)))
             (let ((step2 (merge step1)))
                  step2)))

(define (testdelete i min heap ret)
        (let ((nheap (deleteMin min
                                heap)))
             (let ((_var148 1))
                  (let ((_var149 (eq? _var148
                                      i)))
                       (if _var149
                           ret
                           (let ((nmin (calculateMin nheap)))
                                (let ((_var150 1))
                                     (let ((_var151 (- i
                                                       _var150)))
                                          (let ((_var153 (cons nmin
                                                               ret)))
                                               (let ((_var152 (testdelete _var151
                                                                          nmin
                                                                          nheap
                                                                          _var153)))
                                                    _var152))))))))))

(define (testinsert i min heap)
        (let ((_var155 0))
             (let ((_var156 (eq? _var155
                                 i)))
                  (if _var156
                      (let ((_var157 (cons min
                                           heap)))
                           _var157)
                      (let ((_var158 5))
                           (let ((_var159 (* i
                                             _var158)))
                                (let ((_var161 5))
                                     (let ((_var162 (+ i
                                                       _var161)))
                                          (let ((_var160 (modr _var159
                                                               _var162)))
                                               (let ((nheap (insert _var160
                                                                    heap)))
                                                    (let ((nmin (updateMin min
                                                                           nheap)))
                                                         (let ((_var165 1))
                                                              (let ((_var166 (- i
                                                                                _var165)))
                                                                   (let ((_var167 (testinsert _var166
                                                                                              nmin
                                                                                              nheap)))
                                                                        _var167))))))))))))))

(define (test n)
        (let ((_var168 100))
             (let ((_var170 '()))
                  (let ((_var169 (insert _var168
                                         _var170)))
                       (let ((_var173 '()))
                            (let ((ins (testinsert n
                                                   _var169
                                                   _var173)))
                                 (let ((_var175 (car ins)))
                                      (let ((_var177 (cdr ins)))
                                           (let ((_var179 '()))
                                                (let ((_var176 (testdelete n
                                                                           _var175
                                                                           _var177
                                                                           _var179)))
                                                     _var176))))))))))

(let ((_var181 1000))
     (let ((_var182 (test _var181)))
          _var182))