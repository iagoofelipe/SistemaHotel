#include <map>
#include <iostream>

enum EndPoint {
    GET_ROOM, POST_ROOM, DELETE_ROOM, PUT_ROOM,
    ROOM_BY_ID,
    
    GET_GUEST
};

enum HttpMethod {
    GET, POST, UPDATE, DELETE, PUT
};

struct EndPointProp {
    EndPoint id;
    std::string url;
    HttpMethod method;
};

class HotelAPI {
public:
    const static std::string baseUrl;
    static std::map<EndPoint, EndPointProp> endpoints;

};

const std::string HotelAPI::baseUrl { "http://127.0.0.1:8000/api" };

std::map<EndPoint, EndPointProp> HotelAPI::endpoints {
    { EndPoint::GET_ROOM, { EndPoint::GET_ROOM, "/room", HttpMethod::GET } },
    { EndPoint::POST_ROOM, { EndPoint::POST_ROOM, "/room", HttpMethod::POST } },
    { EndPoint::DELETE_ROOM, { EndPoint::DELETE_ROOM, "/room", HttpMethod::DELETE } },
    { EndPoint::PUT_ROOM, { EndPoint::PUT_ROOM, "/room", HttpMethod::PUT } },
    { EndPoint::ROOM_BY_ID, { EndPoint::ROOM_BY_ID, "/getRoomById", HttpMethod::GET } },
    
    { EndPoint::GET_GUEST, { EndPoint::GET_GUEST, "/guest", HttpMethod::GET } },
};

int main() {

    EndPointProp prop = HotelAPI::endpoints[EndPoint::GET_ROOM];
    std::cout << prop.url << "\t" << prop.id << "\t" << prop.method << std::endl;
}