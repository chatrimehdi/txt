apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: secure-app-ingress
  annotations:
    # CORS Configuration
    nginx.ingress.kubernetes.io/enable-cors: "true"
    nginx.ingress.kubernetes.io/cors-allow-origin: "https://example.com, https://app.example.com"
    nginx.ingress.kubernetes.io/cors-allow-methods: "GET, POST, PUT, DELETE, OPTIONS"
    nginx.ingress.kubernetes.io/cors-allow-headers: "DNT,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Authorization"
    nginx.ingress.kubernetes.io/cors-allow-credentials: "true"
    nginx.ingress.kubernetes.io/cors-max-age: "1728000"
    
    # Secure Cookie Configuration
    nginx.ingress.kubernetes.io/configuration-snippet: |
      proxy_cookie_path / "/; secure; HttpOnly; SameSite=Strict";
      # For multiple cookies or specific cookie names
      proxy_cookie_flags ~ secure httponly samesite=strict;
      
    # Proper Error Handling
    nginx.ingress.kubernetes.io/custom-http-errors: "400,401,403,404,500,502,503,504"
    nginx.ingress.kubernetes.io/default-backend: "default/http-error-pages"
    
    # Security Headers
    nginx.ingress.kubernetes.io/server-snippet: |
      add_header X-Frame-Options "SAMEORIGIN" always;
      add_header X-Content-Type-Options "nosniff" always;
      add_header X-XSS-Protection "1; mode=block" always;
      add_header Referrer-Policy "strict-origin-when-cross-origin" always;
      
    # Rate Limiting for error endpoints
    nginx.ingress.kubernetes.io/limit-rps: "10"
    nginx.ingress.kubernetes.io/limit-burst-multiplier: "5"
    
spec:
  ingressClassName: nginx
  tls:
  - hosts:
    - app.example.com
    secretName: tls-secret
  rules:
  - host: app.example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: app-service
            port:
              number: 80


------------------------------------------------------------------------------------------
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: secure-api-ingress
  annotations:
    # CORS with validation
    nginx.ingress.kubernetes.io/enable-cors: "true"
    nginx.ingress.kubernetes.io/cors-allow-origin: "https://app.example.com"
    nginx.ingress.kubernetes.io/cors-allow-methods: "GET, POST, OPTIONS"
    nginx.ingress.kubernetes.io/cors-allow-headers: "*"
    nginx.ingress.kubernetes.io/cors-expose-headers: "Content-Length,Content-Range"
    nginx.ingress.kubernetes.io/cors-allow-credentials: "true"
    
    # Session security
    nginx.ingress.kubernetes.io/affinity: "cookie"
    nginx.ingress.kubernetes.io/session-cookie-name: "route"
    nginx.ingress.kubernetes.io/session-cookie-expires: "172800"
    nginx.ingress.kubernetes.io/session-cookie-max-age: "172800"
    nginx.ingress.kubernetes.io/session-cookie-samesite: "Strict"
    nginx.ingress.kubernetes.io/session-cookie-secure: "true"
    nginx.ingress.kubernetes.io/session-cookie-httponly: "true"
    
    # Rate limiting
    nginx.ingress.kubernetes.io/limit-connections: "100"
    nginx.ingress.kubernetes.io/limit-rps: "50"
    
    # Timeouts
    nginx.ingress.kubernetes.io/proxy-connect-timeout: "30"
    nginx.ingress.kubernetes.io/proxy-read-timeout: "120"
    nginx.ingress.kubernetes.io/proxy-send-timeout: "120"
    
    # Body size
    nginx.ingress.kubernetes.io/proxy-body-size: "10m"
    
    # Security headers
    nginx.ingress.kubernetes.io/server-snippet: |
      add_header Strict-Transport-Security "max-age=31536000; includeSubDomains" always;
      add_header Content-Security-Policy "default-src 'self'; script-src 'self' 'unsafe-inline' 'unsafe-eval'; style-src 'self' 'unsafe-inline';" always;
      add_header X-Frame-Options "DENY" always;
      add_header X-Content-Type-Options "nosniff" always;
      add_header X-XSS-Protection "1; mode=block" always;
      add_header Referrer-Policy "strict-origin-when-cross-origin" always;
      add_header Permissions-Policy "geolocation=(), microphone=(), camera=()" always;
      
    # Error handling
    nginx.ingress.kubernetes.io/custom-http-errors: "401,403,404,429,500,502,503"
    nginx.ingress.kubernetes.io/default-backend: "error-pages/error-handler"
    
    # Whitelist source ranges (optional)
    nginx.ingress.kubernetes.io/whitelist-source-range: "10.0.0.0/8, 192.168.0.0/16"
    
    # Disable expose headers
    nginx.ingress.kubernetes.io/server-tokens: "false"
    
spec:
  ingressClassName: nginx
  tls:
    - hosts:
        - api.example.com
      secretName: api-tls-cert
  rules:
    - host: api.example.com
      http:
        paths:
          - path: /api
            pathType: Prefix
            backend:
              service:
                name: api-service
                port:
                  number: 8080
          - path: /auth
            pathType: Prefix
            backend:
              service:
                name: auth-service
                port:
                  number: 3000

---------------------------------------------------------------------------------------------------------
2 method
cors origin :
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: my-ingress
  annotations:
    nginx.ingress.kubernetes.io/cors-allow-origin: "https://your-frontend-domain.com"
    nginx.ingress.kubernetes.io/cors-allow-methods: "GET, POST, PUT, DELETE, OPTIONS"
    nginx.ingress.kubernetes.io/cors-allow-headers: "DNT,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range,Authorization"
    nginx.ingress.kubernetes.io/cors-allow-credentials: "true"
spec:
  rules:
  - host: your-api.example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: your-service
            port:
              number: 80

----------------
secure 

metadata:
  annotations:
    nginx.ingress.kubernetes.io/configuration-snippet: |
      proxy_cookie_flags ~ secure httponly samesite=lax;

spec:
  tls:
  - hosts:
    - your-api.example.com
    secretName: your-tls-secret

-----------------

full: 

metadata:
  annotations:
    # CORS
    nginx.ingress.kubernetes.io/cors-allow-origin: "https://your-frontend-domain.com"
    nginx.ingress.kubernetes.io/cors-allow-methods: "GET, POST, PUT, DELETE, OPTIONS"
    nginx.ingress.kubernetes.io/cors-allow-headers: "Content-Type,Authorization"
    nginx.ingress.kubernetes.io/cors-allow-credentials: "true"

    # Secure cookies
    nginx.ingress.kubernetes.io/configuration-snippet: |
      proxy_cookie_flags ~ secure httponly samesite=lax;

    # Security hardening
    nginx.ingress.kubernetes.io/server-snippet: |
      server_tokens off;

    # Optional: hide backend server headers
    nginx.ingress.kubernetes.io/upstream-hide-headers: "Server,X-Powered-By"

-----------------------------------------------------------------
full: ch
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: api-ingress
  annotations:
    nginx.ingress.kubernetes.io/enable-cors: "true"
    nginx.ingress.kubernetes.io/cors-allow-origin: "https://app.example.com"
    nginx.ingress.kubernetes.io/cors-allow-credentials: "true"
    nginx.ingress.kubernetes.io/cors-allow-methods: "GET,POST,PUT,DELETE,OPTIONS"
    nginx.ingress.kubernetes.io/cors-allow-headers: "Authorization,Content-Type"
    nginx.ingress.kubernetes.io/ssl-redirect: "true"
    nginx.ingress.kubernetes.io/server-tokens: "false"
    nginx.ingress.kubernetes.io/custom-http-errors: "400,401,403,404,500"
spec:
  ingressClassName: nginx
  tls:
    - hosts:
        - api.example.com
      secretName: api-tls
  rules:
    - host: api.example.com
      http:
        paths:
          - path: /
            pathType: Prefix
            backend:
              service:
                name: api-service
                port:
                  number: 80

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
                    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


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
