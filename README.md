### Targeted features: 
- Listen and respond to incoming HTTP requests 
- Parse URL routes and return the appropriate response 
- Provide declarative abstractions for defining these URL routes 

### Outcome: 
- All three targeted features were achieved 

### Limitations: 
- As of now, the server can only serve HTML documents. URL routes are mapped directly to HTML documents and aren't generated via View functions. Hence being incompatible with the Model View Controller pattern. 
- Responses are limited to `text/html` mime type. At least supporting `application/json` for JSON responses is a must for real world application of this server. 
- The Server is written in a sequential manner. Ie, it is not written in a reusable fashion. A rewrite in a modular fashion would result in a much better developer experience. 
