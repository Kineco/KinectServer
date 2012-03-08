#include "KinectHttpServer.h"
#include <cstring>

#include <XnCppWrapper.h>

#define HTTP_PORT 8000
#define WS_PORT 8080

int usage(int argc, char * const argv[])
{
 return 0;   
}
int main (int argc, char * const argv[]) {
    int http_port = 0;
    int ws_port = 0;
    
    int i;
    for (i=1;i < argc;i++)
    {
        
        if(!strcmp(argv[i],"--port"))
        {
            http_port = atoi(argv[++i]);
        }
        else if(!strcmp(argv[i],"--wsport"))
        {
            ws_port = atoi(argv[++i]);
        }
        else if(!strcmp(argv[i],"--help"))
        {
            usage(argc,argv);
            return 0;
        }
        
    }
    if(http_port<=0)
        http_port = HTTP_PORT;
    if(ws_port <=0)
        ws_port = WS_PORT;
    
    
    try {
		const char* CONFIG_PATH = "Config/SamplesConfig.xml";
		xn::Context context;
        XnStatus rc = context.InitFromXmlFile(CONFIG_PATH);
        
        if(rc != XN_STATUS_OK){
			throw std::runtime_error(::xnGetStatusString(rc));
		}

        KinectHttpServer kinectServer(http_port, ws_port ,context);
        context.StartGeneratingAll();
        
        std::cout << "KinectServer is running " << std::endl;
        std::cout << " -  HTTP port      : " << http_port << std::endl;
        std::cout << " -  Websocket port : " << ws_port << std::endl;
        
        while(true){
            context.WaitAndUpdateAll();
            kinectServer.updateData();
        }
        
    }catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
        
	}
    
	return 0;
    
}