/*
    This file is part of libquickmail.

    libquickmail is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libquickmail is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libquickmail.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "quickmail.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define FROM        "support@wimd.io"
#define TO          "carlos.tangerino@gmail.com"
//#define CC          "otheruser@domain.com"
//#define BCC         "otheruser@domain.com"
#define SMTPSERVER  "localhost"
#define SMTPPORT    25
#define SMTPUSER    NULL
#define SMTPPASS    NULL

static int sendEmail() {

    char *memAttachment;
    char *gatewayName;
    memAttachment = "file with attachment";
    gatewayName = "Gateway Name";
    char *bodyTemplate =
            "<br><span style='font-family:Arial,Helvetica,sans-serif;font-size:x-large;font-weight:bold;color:#626469'>EDGE Gateway - Meter Data Report</span><br><br>"
            "<span style='font-weight:bold;color:#626469'>Customer Name:&nbsp;</span>"
            "<span>%s (%s)</span><br><br>"
            "<span style='font-weight:bold;color:#626469'>Please, find attached the report that was generated on:&nbsp;</span><span>%s (UTC)</span><br><br><br>"
            "<div style='padding:5px 10px 5px 10px;background-color:#f5f5f5;border-top:solid 1px #cccccc;border-top-style:dashed;font-size:x-small;text-align:center'>"
            "<span>This email is sent from an automated machine. Please do not reply to this email address. E-mails sent from this address are not read or replied</span></div>";
    char *body = malloc(strlen(bodyTemplate) * 2);
    if (!body) {
        return (-1);
    }
    time_t t = time(NULL);
    sprintf(body, bodyTemplate, gatewayName, "00:11:22:33:44:55", asctime(gmtime(&t)));
    quickmail_initialize();
    quickmail mailobj = quickmail_create(FROM, "EDGE Gateway meter data");
    quickmail_add_to(mailobj, TO);
    
    quickmail_add_header(mailobj, "Importance: Low");
    quickmail_add_header(mailobj, "X-Priority: 5");
    quickmail_add_header(mailobj, "X-MSMail-Priority: Low");
    quickmail_add_body_memory(mailobj, "text/html", body, strlen(body), 1);

    const char* errmsg = NULL;
    char *fn = malloc(strlen(gatewayName) * 2 + 256);
    if (fn) {
        sprintf(fn, "%s.%ld.csv", gatewayName, t);
        quickmail_add_attachment_memory(mailobj, fn, NULL, memAttachment, strlen(memAttachment), 0);
        free(fn);
        quickmail_set_debug_log(mailobj, stderr);
        if ((errmsg = quickmail_send(mailobj, "localhost", 0, "", "")) != NULL) {
            printf("Error sending e-mail: %s", errmsg);
        }
    } 
    quickmail_destroy(mailobj);
    if (!errmsg) {
        return 200;
    } else {
        return 0;
    }
}

void list_attachment_callback (quickmail mailobj, const char* filename, quickmail_attachment_open_fn email_info_attachment_open, quickmail_attachment_read_fn email_info_attachment_read, quickmail_attachment_close_fn email_info_attachment_close, void* callbackdata)
{
  printf("[%i]: %s\n", ++*(int*)callbackdata, filename);
}

int main ()
{
    sendEmail();
  return 0;
}
