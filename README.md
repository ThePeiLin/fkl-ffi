fakeLisp的ffi扩展库，编译需要libffi

### 构建
```sh
mkdir build
cd build
cmake ..
make
```

### 示例
```scheme
(import (path to fklffi))
(import (path to lib utils))

(defmacro (ffi-deftype type alias)
  `(ffi-typedef '~type '~alias))

(defmacro (ffi-path-ref path mem,index)
  (begin
    (define iter
      (lambda [c r]
        (cond
          [c (iter (cdr c)
                   `(ffi-ref ~r (quote ~(car c))))]
          [1 r])))
    `(ffi-ref ~(iter path mem) '() ~@index)))
(defmacro (ffi-procedure sym arg-types,rtypes)
  `(ffi-proc ~sym (quote (function ~arg-types ~@rtypes))))

(ffi-typedef
  '(struct
     LinkNode
     (data int)
     (next (ptr (struct LinkNode))))
  'LinkNode)

(ffi-typedef
  '(union
     (i int32_t)
     (d double))
  'Value)

(ffi-sizeof 'LinkNode)

(define create-link-node
  (lambda (data next)
    (define retval (ffi-new 'LinkNode 'raw))
    (ffi-set! (ffi-ref retval 'data) data)
    (ffi-set! (ffi-ref retval 'next) next)
    (ffi-ref retval '&)))

(define head nil)

(let iter-create [(n 0)]
  (when (< n 10)
    (setq head (create-link-node n head))
    (iter-create (1+ n))))

(define traverse-link-node
  (lambda (cur)
    (when (not (ffi-null? cur))
      (princ (ffi-path-ref (* data) cur))
      (traverse-link-node
        (ffi-path-ref (* next) cur)))))

(define ch (go traverse-link-node head))
(recv ch)
(princ #\\n)

(let iter-print [(cur head)]
  (when (not (ffi-null? cur))
    (princ (ffi-path-ref (* data) cur))
    (iter-print
      (ffi-path-ref (* next) cur))))

(princ #\\n)
(apply traverse-link-node head '())
(princ #\\n)

(let iter-free [(cur head)]
  (when (not (ffi-null? cur))
    (define prev cur)
    (setq cur (ffi-new '(ptr LinkNode)))
    (ffi-set! cur (ffi-path-ref (* next) prev))
    (ffi-delete (ffi-path-ref (*) prev))
    (iter-free cur)))
```
