
struct BackendRequestParams
{
    const char* endpoint;
    const char* method; // "GET", "POST", etc.
    const char* body;   // JSON body for POST requests
    const char* headers; // Additional headers if needed
};

class BackendServer {
public:
    
}