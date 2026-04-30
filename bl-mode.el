;;; bl-mode.el --- Major mode for BL language

(defvar bl-keywords
  '("if" "else" "while" "for" "return" "function"))

(defvar bl-types
  '("int"))

(defvar bl-constants
  '("true" "false"))

(defvar bl-font-lock-keywords
  `(
    ;; keywords
    (,(regexp-opt bl-keywords 'words) . font-lock-keyword-face)

    ;; types
    (,(regexp-opt bl-types 'words) . font-lock-type-face)

    ;; constants
    (,(regexp-opt bl-constants 'words) . font-lock-constant-face)

    ;; function names (function foo(...))
    ("function[ \t]+\\([a-zA-Z_][a-zA-Z0-9_]*\\)"
     (1 font-lock-function-name-face))

    ;; function calls
    ("\\<\\([a-zA-Z_][a-zA-Z0-9_]*\\)[ \t]*("
     (1 font-lock-function-name-face))

    ;; numbers
    ("\\b[0-9]+\\b" . font-lock-constant-face)
    ))

;;;###autoload
(require 'cc-mode)

(define-derived-mode bl-mode c-mode "BL"
  "Major mode for editing BL language."

  ;; file extension
  (setq-local tab-width 4)
  (setq-local indent-tabs-mode nil)
  (setq-local c-basic-offset 4)

  ;; your keywords for highlighting (optional, keeps your custom ones)
  (font-lock-add-keywords nil bl-font-lock-keywords)
)

(add-to-list 'auto-mode-alist '("\\.bl\\'" . bl-mode))

;;;###autoload
(add-to-list 'auto-mode-alist '("\\.bl\\'" . bl-mode))

(provide 'bl-mode)
