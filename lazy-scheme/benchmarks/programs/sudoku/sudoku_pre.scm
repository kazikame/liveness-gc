(define ( and a b)
 (if a 
 (if b #t 
 #f ) 
 #f))

(define ( or a b)
 (if a a 
 b 
 ))

(define (list listx ) (cons listx () ) ) 

(define (id idx ) idx ) 

(define (firstcol fcx ) 
(if (null? fcx ) () 
(cons (car (car fcx ) ) 
(firstcol (cdr fcx ) ) ) ) ) 

(define (rest rx ) 
(if (null? rx ) () 
(cons (cdr (car rx ) ) 
(rest (cdr rx ) ) ) ) ) 

(define (transpose tx ) 
(if (null? (car tx ) ) () 
(cons (firstcol tx ) 
(transpose (rest tx ) ) ) ) ) 

(define (append ax ay ) 
(if (null? ax ) ay 
(cons (car ax ) 
(append (cdr ax ) ay ) ) ) ) 

(define (concat cx ) 
(if (null? cx ) () 
(append (car cx ) 
(concat (cdr cx ) ) ) ) ) 

(define (length lx ) 
(if (null? lx ) 0 
(+ 1 (length (cdr lx ) ) ) ) ) 

(define (take tkn tkx ) 
(if (eq? tkn 0 ) () 
(cons (car tkx ) 
(take (- tkn 1 ) (cdr tkx ) ) ) ) ) 

(define (drop dn dx ) 
(if (eq? dn 0 ) dx 
(drop (- dn 1 ) (cdr dx ) ) ) ) 

(define (and anx any ) (if (eq? anx #t ) any #f ) ) 

(define (or orx ory ) (if (eq? orx #t ) orx ory ) ) 

(define (not ntx ) (if ntx #f #t ) ) 

(define (empty ex ) (eq? 0 ex ) ) 

(define (single sx ) (if (eq? (length sx ) 1 ) #t #f ) ) 

(define (map-cols mcx ) 
(if (null? mcx ) () 
(cons (cols (car mcx ) ) 
(map-cols (cdr mcx ) ) ) ) ) 

(define (rows rsx ) (id rsx ) ) 

(define (cols csx ) (transpose csx ) ) 

(define (boxes bsx ) (unpack (map-cols (pack bsx ) ) ) ) 

(define (map-split msx ) 
(if (null? msx ) () 
(cons (split (car msx ) ) 
(map-split (cdr msx ) ) ) ) ) 

(define (map-concat mcox ) 
(if (null? mcox ) () 
(cons (concat (car mcox ) ) 
(map-concat (cdr mcox ) ) ) ) ) 

(define (unpack ux ) (map-concat (concat ux ) ) ) 

(define (chop chn chx ) (if (null? chx ) chx (cons (take chn chx ) (chop chn (drop chn chx ) ) ) ) ) 

(define (pack pax ) (split (map-split pax ) ) ) 

(define (elem elv elx ) 
(if (null? elx ) #f 
(if (eq? (car elx ) elv ) #t (elem elv (cdr elx ) ) ) ) ) 

(define (alltrue atx ) 
(if (null? atx ) #t 
(if (eq? (car atx ) #f ) #f (alltrue (cdr atx ) ) ) ) ) 

(define (nodups ndx ) 
(if (null? ndx ) #t 
(and (not (elem (car ndx ) (cdr ndx ) ) ) (nodups (cdr ndx ) ) ) ) ) 

(define (map-nodups mndx ) 
(if (null? mndx ) () 
(cons (nodups (car mndx ) ) 
(map-nodups (cdr mndx ) ) ) ) ) 

; (define (all-nodups andx ) 
; (alltrue (map-nodups andx ) ) ) 

; (define (valid vg ) (and (all-nodups (rows vg ) ) 
; (and (all-nodups (cols vg ) ) 
; (all-nodups (boxes vg ) ) ) ) ) 

(define (map-choice mchx ) 
(if (null? mchx ) () 
(cons (choice (car mchx ) ) 
(map-choice (cdr mchx ) ) ) ) ) 

(define (map-map-choice mmchx ) 
(if (null? mmchx ) () 
(cons (map-choice (car mmchx ) ) 
(map-map-choice (cdr mmchx ) ) ) ) ) 

(define (choices chsx ) (map-map-choice chsx ) ) 

(define (map-cp mcpx ) 
(if (null? mcpx ) 
() 
(cons (cp (car mcpx ) ) 
(map-cp (cdr mcpx ) ) ) ) ) 

(define (cp cpx ) (if (null? cpx ) (list () ) 
		 (concat (cphelp (car cpx ) (cp (cdr cpx ) ) ) ) ) ) 

(define (cphelp cphx cphy ) (if (null? cphx ) () 
			 (cons (cphelp2 (car cphx ) cphy ) 
			 (cphelp (cdr cphx ) cphy ) ) ) ) 

(define (cphelp2 cph2x cph2y ) (if (null? cph2y ) () 
			 (cons (cons cph2x (car cph2y ) ) 
				 (cphelp2 cph2x (cdr cph2y ) ) ) ) ) 

(define (collapse cex ) (cp (map-cp cex ) ) ) 

(define (minus mx my ) (if (single mx ) mx (minushelp mx my ) ) ) 

(define (minushelp mshx mshy ) 
(if (null? mshy ) mshx 
(minushelp (remove (car mshy ) mshx ) (cdr mshy ) ) ) ) 

(define (remove rmvv rmvx ) 
(if (null? rmvx ) rmvx 
(if (eq? (car rmvx ) rmvv ) 
(remove rmvv (cdr rmvx ) ) 
(cons (car rmvx ) (remove rmvv (cdr rmvx ) ) ) ) ) ) 

(define (filter-single fslx ) (if (null? fslx ) () 
			 (if (single (car fslx ) ) 
			 (cons (car fslx ) (filter-single (cdr fslx ) ) ) 
			 (filter-single (cdr fslx ) ) ) ) ) 

(define (reduce redx ) (let ((singles (concat (filter-single redx ) ) ) ) 
		 (reducehelp redx singles ) ) ) 

(define (reducehelp redhx rsingles ) 
		 (if (null? redhx ) 
			 () 
			 (cons (minus (car redhx ) rsingles ) 
			 (reducehelp (cdr redhx ) rsingles ) ) ) ) 

(define (prune prux ) (pruneby-boxes (pruneby-cols (pruneby-rows prux ) ) ) ) 

(define (map-reduce mredx ) 
(if (null? mredx ) () 
(cons (reduce (car mredx ) ) (map-reduce (cdr mredx ) ) ) ) ) 

(define (pruneby-boxes prubx ) 
(boxes (map-reduce (boxes prubx ) ) ) ) 

(define (pruneby-cols prucx ) 
(cols (map-reduce (cols prucx ) ) ) ) 

(define (pruneby-rows prurx ) 
(rows (map-reduce (rows prurx ) ) ) ) 

(define (all-single asex ) 
(alltrue (map-single asex ) ) ) 

(define (map-single msex ) 
(if (null? msex ) () 
(cons (single (car msex ) ) 
(map-single (cdr msex ) ) ) ) ) 

(define (all-all-single aasex ) 
(alltrue (map-all-single aasex ) ) ) 

(define (map-all-single masex ) 
(if (null? masex ) () 
(cons (all-single (car masex ) ) 
(map-all-single (cdr masex ) ) ) ) ) 

(define (complete cmex ) (all-all-single cmex ) ) 

(define (any-null anlx ) 
(if (null? anlx ) #f 
(if (null? (car anlx ) ) #t 
(any-null (cdr anlx ) ) ) ) ) 

(define (any-any-null aanlx ) 
(if (null? aanlx ) #f 
(if (any-null (car aanlx ) ) #t 
(any-any-null (cdr aanlx ) ) ) ) ) 

(define (void vx ) (any-any-null vx ) ) 

(define (all-consistent actx ) (alltrue (map-consistent actx ) ) ) 

(define (map-consistent mctx ) 
(if (null? mctx ) () 
(cons (consistent (car mctx ) ) 
(map-consistent (cdr mctx ) ) ) ) ) 

(define (consistent ctx ) (nodups (concat (filter-single ctx ) ) ) ) 

(define (safe sfex ) (and (all-consistent (rows sfex ) ) 
		 (and (all-consistent (cols sfex ) ) 
			 (all-consistent (boxes sfex ) ) ) ) ) 

(define (blocked bkdx ) (or (null? bkdx ) 
(or (void bkdx ) 
(not (safe bkdx ) ) ) ) ) 

(define (search m ) 
(if (blocked m ) () 
(if (complete m ) (collapse m ) 
	 (let ((m1 (expand m ) ) ) 
(searchhelp m1 ) ) ) ) ) 

(define (searchhelp m2 ) 
(if (null? m2 ) () 
(let ((s2 (search (prune (car m2 ) ) ) ) ) 
(if (single s2 ) 
s2 
(searchhelp (cdr m2 ) ) ) ) ) ) 

(define (not-single nsx ) (not (single nsx ) ) ) 

(define (any-not-single ansx ) 
(if (null? ansx ) #f 
(if (not-single (car ansx ) ) #t 
(any-not-single (cdr ansx ) ) ) ) ) 

(define (until-any-not-single uansx ) 
(if (null? uansx ) () 
(if (any-not-single (car uansx ) ) () 
(cons (car uansx ) 
(until-any-not-single (cdr uansx ) ) ) ) ) ) 

(define (after-any-not-single aansx ) 
(if (null? aansx ) () 
(if (any-not-single (car aansx ) ) aansx 
(after-any-not-single (cdr aansx ) ) ) ) ) 

(define (until-not-single unsx ) 
(if (null? unsx ) () 
(if (not-single (car unsx ) ) () 
(cons (car unsx ) 
(until-not-single (cdr unsx ) ) ) ) ) ) 

(define (after-not-single afnsx ) 
(if (null? afnsx ) () 
(if (not-single (car afnsx ) ) afnsx 
(after-not-single (cdr afnsx ) ) ) ) ) 

(define (expand em ) 
(let ((vrows1 (until-any-not-single em ) ) ) 
(let ((vrows2tmp (after-any-not-single em ) ) ) 
(let ((vrow (car vrows2tmp ) ) ) 
	 (let ((vrows2 (cdr vrows2tmp ) ) ) 
	 (let ((vrow1 (until-not-single vrow ) ) ) 
	 (let ((vrow2tmp (after-not-single vrow ) ) ) 
	 (let ((vcs (car vrow2tmp ) ) ) 
		 (let ((vrow2 (cdr vrow2tmp ) ) ) 
		 (expandhelp vrows1 vrow1 vrow2 vrows2 vcs ) ) ) ) ) ) ) ) ) ) 

(define (expandhelp vrs1 vr1 vr2 vrs2 ehx ) 
(if (null? ehx ) () 
(let ((vnr (append vr1 (cons (list (car ehx ) ) vr2 ) ) ) ) 
(let ((vnb (append vrs1 (cons vnr vrs2 ) ) ) ) 
(cons vnb (expandhelp vrs1 vr1 vr2 vrs2 (cdr ehx ) ) ) ) ) ) ) 

(define (solve slvx ) (search (prune (choices slvx ) ) ) ) 


(define (split spx ) 
(let ((boxsize 3 ) ) 
(chop boxsize spx ) 
) ) 

; (define (choice chv ) 
; (let ((vals (cons 1 (cons 2 (cons 3 (cons 4 (cons 5 (cons 6 (cons 7 (cons 8 (cons 9 () ) ) ) ) ) ) ) ) ) ) ) 
; (if (empty chv ) vals (list chv ) ) ) ) 

(define (choice chv ) 
(let ((vals (cons 1 (cons 2 (cons 3 (cons 4 () ) ) ) ) ) ) 
(if (empty chv ) vals (list chv ) ) ) ) 


(let ((b1 (cons (cons 8 (cons 0 (cons 0 (cons 0 (cons 0 (cons 0 (cons 0 (cons 0 (cons 0 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 0 (cons 3 (cons 6 (cons 0 (cons 0 (cons 0 (cons 0 (cons 8 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 7 (cons 0 (cons 0 (cons 9 (cons 0 (cons 2 (cons 0 (cons 0 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 5 (cons 0 (cons 0 (cons 0 (cons 7 (cons 0 (cons 0 (cons 0 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 0 (cons 0 (cons 0 (cons 4 (cons 5 (cons 7 (cons 0 (cons 0 () ) ) ) ) ) ) ) ) ) 
(cons (cons 6 (cons 0 (cons 0 (cons 1 (cons 0 (cons 4 (cons 0 (cons 3 (cons 0 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 0 (cons 1 (cons 0 (cons 0 (cons 0 (cons 0 (cons 6 (cons 8 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 4 (cons 8 (cons 5 (cons 0 (cons 0 (cons 0 (cons 1 (cons 0 () ) ) ) ) ) ) ) ) ) 
(cons (cons 0 (cons 9 (cons 7 (cons 0 (cons 0 (cons 0 (cons 4 (cons 0 (cons 0 () ) ) ) ) ) ) ) ) ) 
() ) ) ) ) ) ) ) ) ) 
) ) 
(solve b1 ) 
) 

; (let ((b1 (cons (cons 1 (cons 3 (cons 2 (cons 4 () ) ) ) ) 
; (cons (cons 2 (cons 4 (cons 1 (cons 3 () ) ) ) ) 
; (cons (cons 3 (cons 1 (cons 4 (cons 2 () ) ) ) ) 
; (cons (cons 4 (cons 2 (cons 3 (cons 0 () ) ) ) ) 
; () ) ) ) ) 
; ) ) 
; (solve b1 ) 
; ) 
