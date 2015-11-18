(define (append l1 l2)
        (let ((_var0 (null? l1)))
             (if _var0
                 l2
                 (let ((_var1 (car l1)))
                      (let ((_var3 (cdr l1)))
                           (let ((_var4 (append _var3
                                                l2)))
                                (let ((_var2 (cons _var1
                                                   _var4)))
                                     _var2)))))))

(define (member x l1)
        (let ((_var6 (null? l1)))
             (if _var6
                 (let ((_var7 #f))
                      _var7)
                 (let ((_var8 (car l1)))
                      (let ((_var9 (eq? x
                                        _var8)))
                           (if _var9
                               (let ((_var10 #t))
                                    _var10)
                               (let ((_var11 (cdr l1)))
                                    (let ((_var12 (member x
                                                          _var11)))
                                         _var12))))))))

(define (list1 x)
        (let ((_var13 '()))
             (let ((_var14 (cons x
                                 _var13)))
                  _var14)))

(define (list2 x y)
        (let ((_var15 (list1 y)))
             (let ((_var16 (cons x
                                 _var15)))
                  _var16)))

(define (list3 x y z)
        (let ((_var17 (list2 y
                             z)))
             (let ((_var18 (cons x
                                 _var17)))
                  _var18)))

(define (list4 x y z w)
        (let ((_var19 (list3 y
                             z
                             w)))
             (let ((_var20 (cons x
                                 _var19)))
                  _var20)))

(define (make-leaf symbol weight)
        (let ((_var21 "leaf"))
             (let ((_var22 (list3 _var21
                                  symbol
                                  weight)))
                  _var22)))

(define (leaf? object)
        (let ((_var23 (car object)))
             (let ((_var25 "leaf"))
                  (let ((_var24 (eq? _var23
                                     _var25)))
                       _var24))))

(define (symbol-leaf x)
        (let ((_var27 (cdr x)))
             (let ((_var28 (car _var27)))
                  _var28)))

(define (weight-leaf x)
        (let ((_var29 (cdr x)))
             (let ((_var30 (cdr _var29)))
                  (let ((_var31 (car _var30)))
                       _var31))))

(define (make-code-tree left right)
        (let ((_var32 (symbols left)))
             (let ((_var34 (symbols right)))
                  (let ((_var33 (append _var32
                                        _var34)))
                       (let ((_var37 (weight left)))
                            (let ((_var39 (weight right)))
                                 (let ((_var38 (+ _var37
                                                  _var39)))
                                      (let ((_var36 (list4 left
                                                           right
                                                           _var33
                                                           _var38)))
                                           _var36))))))))

(define (left-branch tree)
        (let ((_var42 (car tree)))
             _var42))

(define (right-branch tree)
        (let ((_var43 (cdr tree)))
             (let ((_var44 (car _var43)))
                  _var44)))

(define (symbols tree)
        (let ((_var45 (leaf? tree)))
             (if _var45
                 (let ((_var46 (symbol-leaf tree)))
                      (let ((_var47 (list1 _var46)))
                           _var47))
                 (let ((_var48 (cdr tree)))
                      (let ((_var49 (cdr _var48)))
                           (let ((_var50 (car _var49)))
                                _var50))))))

(define (weight tree)
        (let ((_var51 (leaf? tree)))
             (if _var51
                 (let ((_var52 (weight-leaf tree)))
                      _var52)
                 (let ((_var53 (cdr tree)))
                      (let ((_var54 (cdr _var53)))
                           (let ((_var55 (cdr _var54)))
                                (let ((_var56 (car _var55)))
                                     _var56)))))))

(define (decode-1 bits current-branch tree)
        (let ((_var57 (null? bits)))
             (if _var57
                 (let ((_var58 '()))
                      _var58)
                 (let ((_var59 (car bits)))
                      (let ((next-branch (choose-branch _var59
                                                        current-branch)))
                           (let ((_var61 (leaf? next-branch)))
                                (if _var61
                                    (let ((_var62 (symbol-leaf next-branch)))
                                         (let ((_var64 (cdr bits)))
                                              (let ((_var65 (decode-1 _var64
                                                                      tree
                                                                      tree)))
                                                   (let ((_var63 (cons _var62
                                                                       _var65)))
                                                        _var63))))
                                    (let ((_var67 (cdr bits)))
                                         (let ((_var68 (decode-1 _var67
                                                                 next-branch
                                                                 tree)))
                                              _var68)))))))))

(define (decode bits tree)
        (let ((_var69 (decode-1 bits
                                tree
                                tree)))
             _var69))

(define (choose-branch bit branch)
        (let ((_var70 0))
             (let ((_var71 (= bit
                              _var70)))
                  (if _var71
                      (let ((_var72 (left-branch branch)))
                           _var72)
                      (let ((_var73 (right-branch branch)))
                           _var73)))))

(define (adjoin-set x set)
        (let ((_var74 (null? set)))
             (if _var74
                 (let ((_var75 (list1 x)))
                      _var75)
                 (let ((_var76 (weight x)))
                      (let ((_var78 (car set)))
                           (let ((_var79 (weight _var78)))
                                (let ((_var77 (< _var76
                                                 _var79)))
                                     (if _var77
                                         (let ((_var81 (cons x
                                                             set)))
                                              _var81)
                                         (let ((_var82 (car set)))
                                              (let ((_var84 (cdr set)))
                                                   (let ((_var85 (adjoin-set x
                                                                             _var84)))
                                                        (let ((_var83 (cons _var82
                                                                            _var85)))
                                                             _var83))))))))))))

(define (make-leaf-set pairs)
        (let ((_var87 (null? pairs)))
             (if _var87
                 (let ((_var88 '()))
                      _var88)
                 (let ((pair (car pairs)))
                      (let ((_var89 (car pair)))
                           (let ((_var91 (cdr pair)))
                                (let ((_var92 (car _var91)))
                                     (let ((_var90 (make-leaf _var89
                                                              _var92)))
                                          (let ((_var95 (cdr pairs)))
                                               (let ((_var96 (make-leaf-set _var95)))
                                                    (let ((_var94 (adjoin-set _var90
                                                                              _var96)))
                                                         _var94)))))))))))

(define (encode message tree)
        (let ((_var98 (null? message)))
             (if _var98
                 (let ((_var99 '()))
                      _var99)
                 (let ((_var100 (car message)))
                      (let ((_var101 (encode-symbol _var100
                                                    tree)))
                           (let ((_var103 (cdr message)))
                                (let ((_var104 (encode _var103
                                                       tree)))
                                     (let ((_var102 (append _var101
                                                            _var104)))
                                          _var102))))))))

(define (encode-symbol symbol tree)
        (let ((_var106 (leaf? tree)))
             (if _var106
                 (let ((_var107 '()))
                      _var107)
                 (let ((_var108 (left-branch tree)))
                      (let ((_var109 (symbols _var108)))
                           (let ((_var110 (member symbol
                                                  _var109)))
                                (if _var110
                                    (let ((_var111 0))
                                         (let ((_var113 (left-branch tree)))
                                              (let ((_var114 (encode-symbol symbol
                                                                            _var113)))
                                                   (let ((_var112 (cons _var111
                                                                        _var114)))
                                                        _var112))))
                                    (let ((_var116 1))
                                         (let ((_var118 (right-branch tree)))
                                              (let ((_var119 (encode-symbol symbol
                                                                            _var118)))
                                                   (let ((_var117 (cons _var116
                                                                        _var119)))
                                                        _var117)))))))))))

(define (generate-huffman-tree pairs)
        (let ((_var121 (make-leaf-set pairs)))
             (let ((_var122 (successive-merge _var121)))
                  _var122)))

(define (successive-merge leafs)
        (let ((_var123 (cdr leafs)))
             (let ((_var124 (null? _var123)))
                  (if _var124
                      (let ((_var125 (car leafs)))
                           _var125)
                      (let ((_var126 (car leafs)))
                           (let ((_var128 (cdr leafs)))
                                (let ((_var129 (car _var128)))
                                     (let ((_var127 (make-code-tree _var126
                                                                    _var129)))
                                          (let ((_var132 (cdr leafs)))
                                               (let ((_var133 (cdr _var132)))
                                                    (let ((_var131 (adjoin-set _var127
                                                                               _var133)))
                                                         (let ((_var135 (successive-merge _var131)))
                                                              _var135))))))))))))

(define (print-tree tree)
        tree)

(define (make-string n)
        (let ((_var136 0))
             (let ((_var137 (= n
                               _var136)))
                  (if _var137
                      (let ((_var138 '()))
                           _var138)
                      (let ((_var139 "A"))
                           (let ((_var141 1))
                                (let ((_var142 (- n
                                                  _var141)))
                                     (let ((_var143 (make-string _var142)))
                                          (let ((_var140 (cons _var139
                                                               _var143)))
                                               _var140)))))))))

(let ((_var145 "A"))
     (let ((_var147 4))
          (let ((_var146 (make-leaf _var145
                                    _var147)))
               (let ((_var150 "B"))
                    (let ((_var152 2))
                         (let ((_var151 (make-leaf _var150
                                                   _var152)))
                              (let ((_var155 "D"))
                                   (let ((_var157 1))
                                        (let ((_var156 (make-leaf _var155
                                                                  _var157)))
                                             (let ((_var160 "C"))
                                                  (let ((_var162 1))
                                                       (let ((_var161 (make-leaf _var160
                                                                                 _var162)))
                                                            (let ((_var159 (make-code-tree _var156
                                                                                           _var161)))
                                                                 (let ((_var154 (make-code-tree _var151
                                                                                                _var159)))
                                                                      (let ((sample-tree (make-code-tree _var146
                                                                                                         _var154)))
                                                                           (let ((_var167 0))
                                                                                (let ((_var169 1))
                                                                                     (let ((_var171 1))
                                                                                          (let ((_var173 0))
                                                                                               (let ((_var175 0))
                                                                                                    (let ((_var177 1))
                                                                                                         (let ((_var179 0))
                                                                                                              (let ((_var181 1))
                                                                                                                   (let ((_var183 0))
                                                                                                                        (let ((_var185 1))
                                                                                                                             (let ((_var187 1))
                                                                                                                                  (let ((_var189 1))
                                                                                                                                       (let ((_var191 0))
                                                                                                                                            (let ((_var193 '()))
                                                                                                                                                 (let ((_var192 (cons _var191
                                                                                                                                                                      _var193)))
                                                                                                                                                      (let ((_var190 (cons _var189
                                                                                                                                                                           _var192)))
                                                                                                                                                           (let ((_var188 (cons _var187
                                                                                                                                                                                _var190)))
                                                                                                                                                                (let ((_var186 (cons _var185
                                                                                                                                                                                     _var188)))
                                                                                                                                                                     (let ((_var184 (cons _var183
                                                                                                                                                                                          _var186)))
                                                                                                                                                                          (let ((_var182 (cons _var181
                                                                                                                                                                                               _var184)))
                                                                                                                                                                               (let ((_var180 (cons _var179
                                                                                                                                                                                                    _var182)))
                                                                                                                                                                                    (let ((_var178 (cons _var177
                                                                                                                                                                                                         _var180)))
                                                                                                                                                                                         (let ((_var176 (cons _var175
                                                                                                                                                                                                              _var178)))
                                                                                                                                                                                              (let ((_var174 (cons _var173
                                                                                                                                                                                                                   _var176)))
                                                                                                                                                                                                   (let ((_var172 (cons _var171
                                                                                                                                                                                                                        _var174)))
                                                                                                                                                                                                        (let ((_var170 (cons _var169
                                                                                                                                                                                                                             _var172)))
                                                                                                                                                                                                             (let ((sample-message (cons _var167
                                                                                                                                                                                                                                         _var170)))
                                                                                                                                                                                                                  (let ((_var207 50000))
                                                                                                                                                                                                                       (let ((_var208 (make-string _var207)))
                                                                                                                                                                                                                            (let ((_var209 (encode _var208
                                                                                                                                                                                                                                                   sample-tree)))
                                                                                                                                                                                                                                 (let ((enc-dec (decode _var209
                                                                                                                                                                                                                                                        sample-tree)))
                                                                                                                                                                                                                                      enc-dec))))))))))))))))))))))))))))))))))))))))))))))
