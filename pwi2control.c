#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <libxml/xpathInternals.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#include <time.h>
#include <unistd.h>


int callTCP();

char  *strip(char *str)
{
	char *end;
	while(isspace(*str)) str++;
	if(*str == 0)
	return NULL;
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;
	*(end + 1) = 0;
	return str;
}

static void print_text_nodes(xmlNode *a_node)
{
	xmlNode *cur_node = NULL;
	xmlChar *content;
        for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{
		if (cur_node->type == XML_TEXT_NODE) 
		{
			content = strip(xmlNodeGetContent(cur_node));
            
			if (content != NULL)
			{
				//printf("%s: %s\n", cur_node->parent->name, content);
			}
		}

		print_text_nodes(cur_node->children);
	}
}

xmlDocPtr getdoc (char *docname) 
{       xmlDocPtr doc;
	doc = xmlParseFile(docname);
	if (doc == NULL ) 
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}
	return doc;
}

xmlXPathObjectPtr getnodeset (xmlDocPtr doc, xmlChar *xpath, xmlChar *ns_prefix, xmlChar *ns_uri) 
{
	int status;
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;
	context = xmlXPathNewContext(doc);
	if (context == NULL) 
	{
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	xmlXPathRegisterNs(context, ns_prefix, ns_uri);
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) 
	{
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}
	return result;
}

int callTCP ()
{
	char *docname = "http://192.168.100.3:8080/?cmd=getsystem", *id, *color, *value;
	int i, size, num_children;
	xmlXPathObjectPtr result;
	xmlNodeSetPtr nodeset;
	xmlChar *keyword;
	xmlNodePtr node;
	xmlDocPtr doc;
	int azimuth_value; 

        static int previous_value; 
                     
        //printf("\n Previous Value %d\n",previous_value);
	doc = getdoc(docname);
	result = getnodeset(doc, BAD_CAST "/system/mount/azm_encoder_degs", NULL, NULL);
        if (result)
	{     
		for(i = 0; i < result->nodesetval->nodeNr; i++)
		{  
			node = result->nodesetval->nodeTab[i];
			num_children = (int) xmlChildElementCount(node);
			id = xmlGetProp(node, BAD_CAST "id");
                       value = strip(xmlNodeGetContent(node));
		       sscanf(value,"%d",&azimuth_value);
                      
                       printf("Telescope Azimuth   %d \n",  azimuth_value);
                	          

            if(previous_value<azimuth_value)
                        printf("\n Turning Right \n");
              else if (previous_value > azimuth_value)
                printf("\n Turning Left \n");
              else if(previous_value==azimuth_value)
               printf("\n Dome Aligned \n");
		}
        
		xmlXPathFreeObject(result);
    }
           
          previous_value=azimuth_value;
         
                         
                       
			
      
	xmlFreeDoc(doc);
	xmlCleanupParser();
    
	return(0);
}

int main()
{
for (;;) {
         system("clear"); 
        double interval = 2;

        time_t start = time(NULL);

        int duration = callTCP() % 13;
        
        sleep(duration); 
       
        time_t end = time(NULL);

        
        double elapsed = difftime(end, start);
        int seconds_to_sleep = (int)(interval - elapsed);
        if (seconds_to_sleep > 0) { 
            sleep(seconds_to_sleep);
        }
    }
return 0;
}
