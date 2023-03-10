# A HTTP Server in C
This was for my Semester 1 Mini Project for the course Programming in C. I picked this to be my problem statement as i wanted to explore how webservers operate at a low level after being exposed to abstractions like Flask and Express all my life. This was an exceptional learning experience and a project that got be insanely excited about low level programming in general!

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
