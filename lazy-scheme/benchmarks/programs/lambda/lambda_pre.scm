(define ( and a b)
 (if a 
 (if b #t 
 #f ) 
 #f))

;conversion of scheme "equal?" 
(define (isEqual lst1 lst2 ) 
(if (null? lst1 ) 
(if (null? lst2 ) 
#t 
#f 
) 
(if (null? lst2 ) 
(if (null? lst1 ) 
#t 
#f ) 
(if (eq? (car lst1 ) (car lst2 ) ) 
(isEqual (cdr lst1 ) (cdr lst2 ) ) 
#f 
) ) ) 
) 



;; data Term 
;; = Var String 
;; | Con Int 
;; | Incr 
;; | Add Term Term 
;; | Lam String Term 
;; | App Term Term 
;; | IfZero Term Term Term 
;; -- the following terms are used internally 
;; | Thunk Term Env -- a closure 
;; deriving (Eq,Read,Show ) 

;; make and break == constructor and destructors 
(define (mkVar s ) 
(cons "Var" (cons s ' () ) ) ) 

(define (mkCon i ) 
(cons "Con" (cons i ' () ) ) ) 

;;TODO: type checking may be added 
(define (bkCon c ) 
(car (cdr c ) ) ) 

(define (mkIncr ) 
(cons "Incr" ' () ) ) 

(define (mkAdd t1 t2 ) 
(cons "Add" (cons t1 (cons t2 ' () ) ) ) ) 

(define (mkLam s t ) 
(cons "Lam" (cons s (cons t ' () ) ) ) ) 

(define (mkApp t1 t2 ) 
(cons "App" (cons t1 (cons t2 ' () ) ) ) ) 

(define (mkIfZero t1 t2 t3 ) 
(cons "IfZero" (cons t1 (cons t2 (cons t3 ' () ) ) ) ) ) 

;;-- the following terms are used internally 
(define (mkThunk t e ) 
(cons "Thunk" (cons t (cons e ' () ) ) ) ) 

;; ;; type Env = [ (String,Term ) ] 
(define (mkEnvPair s t ) 
(cons s (cons t ' () ) ) ) 

(define (addToEnv e env ) 
(cons e env ) ) 

(define (lookup s v ) 
(if (null? s ) 
(cons "error1-unknown-variable" 1 ) ; (error "Error: unknown variable" ) 
(let ((elt (car s ) ) ) 
	 (if (eq? v (car elt ) ) 
	 (car (cdr elt ) ) 
	 (lookup (cdr s ) v ) ) ) ) ) 

;; simple evluation of lambda expressions 
;; simpleEval :: Env -> Term -> Term 
(define (simpleEval env p ) 
(let ((tag (car p ) ) ) 
(if (eq? "Var" tag ) 
	 (simpleEval env (lookup env (car (cdr p ) ) ) ) 
	 (if (eq? "Add" tag ) 
	 (let ((u (simpleEval env (car (cdr p ) ) ) ) ) 
	 (let ((v (simpleEval env (car (cdr (cdr p ) ) ) ) ) ) 
		 (mkCon (+ (bkCon u ) (bkCon v ) ) ) ) ) 
	 (if (eq? "Thunk" tag ) 
		 (simpleEval (car (cdr (cdr p ) ) ) (car (cdr p ) ) ) 
		 (if (eq? "Lam" tag ) 
		 (mkThunk p env ) 
		 (if (eq? "App" tag ) 
			 (let ((u (simpleEval env (car (cdr p ) ) ) ) ) 
			 (simpleApply env u (car (cdr (cdr p ) ) ) ) ) 
			 (if (eq? "IfZero" tag ) 
			 (let ((val (simpleEval env (car (cdr p ) ) ) ) ) 
			 (if (isEqual (mkCon 0 ) val ) 
				 (simpleEval env (car (cdr (cdr p ) ) ) ) 
				 (simpleEval env (car (cdr (cdr (cdr p ) ) ) ) ) ) ) 
			 (if (eq? "Con" tag ) 
				p 
				 (if (eq? "Incr" tag ) 
				 (mkCon 0 ) 
				 (cons "error2-in-simpleEval" p ) ; (error "error in simpleEval:" p ) 
) ) ) ) ) ) ) ) ) ) 

;; simpleApply :: Env -> Term -> Term -> Id Term 
(define (simpleApply env t a ) 
(let ((tag (car t ) ) ) 
(if (eq? "Thunk" tag ) 
	 (let ((x (car (cdr (car (cdr t ) ) ) ) ) ) 
	 (let ((b (car (cdr (cdr (car (cdr t ) ) ) ) ) ) ) 
	 (let ((e (car (cdr (cdr t ) ) ) ) ) 
	 (let ((env2 (addToEnv (mkEnvPair x (mkThunk a env ) ) e ) ) ) 
		 (simpleEval env2 b ) ) ) ) ) 
	 (cons "error3-cannot-simpleApply" 3 ) ; (error "Cannot simpleApply" env t a ) 
) ) ) 

;; main program and test case 
(define (simpleMain arg ) 
(simpleEval ' () (pgm arg ) ) ) 

(define (pgm arg ) 
(mkApp (sum0 ) (mkCon arg ) ) ) 

(define (nMinus1 ) (mkAdd (mkVar "n" ) (mkCon (- 0 1) ) ) ) 

(define (partialSum0 ) 
(mkLam "sum" 
	 (mkLam "n" 
		 (mkIfZero (mkVar "n" ) 
			 (mkCon 0 ) 
			 (mkAdd (mkVar "n" ) (mkApp (mkVar "sum" ) (nMinus1 ) ) ) ) ) ) ) 

(define (lfxx ) 
(mkLam "x" (mkApp (mkVar "F" ) (mkApp (mkVar "x" ) (mkVar "x" ) ) ) ) ) 

;; This is the fix point combinator: Y 
(define (fix ) (mkLam "F" (mkApp (lfxx ) (lfxx ) ) ) ) 

(define (sum0 ) 
(mkApp (fix ) (partialSum0 ) ) 
) 

(simpleMain 500 ) 
