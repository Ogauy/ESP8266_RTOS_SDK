/* ESP HTTP Client Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <stdlib.h>

#include "../../get_weather/main/app_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_http_client.h"

#if CONFIG_SSL_USING_WOLFSSL
#include "lwip/apps/sntp.h"
#endif

#define MAX_HTTP_RECV_BUFFER 1023
static const char *TAG = "HTTP_CLIENT";

/* Root cert for howsmyssl.com, taken from howsmyssl_com_root_cert.pem

   The PEM file was extracted from the output of this command:
   openssl s_client -showcerts -connect www.howsmyssl.com:443 </dev/null

   The CA root cert is the last cert given in the chain of certs.

   To embed it in the app binary, the PEM file is named
   in the component.mk COMPONENT_EMBED_TXTFILES variable.
*/
extern const char howsmyssl_com_root_cert_pem_start[] asm("_binary_howsmyssl_com_root_cert_pem_start");
extern const char howsmyssl_com_root_cert_pem_end[]   asm("_binary_howsmyssl_com_root_cert_pem_end");

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // Write out data
                // printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

#if CONFIG_SSL_USING_WOLFSSL
static void get_time()
{
    struct timeval now;
    int sntp_retry_cnt = 0;
    int sntp_retry_time = 0;

    sntp_setoperatingmode(0);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    while (1) {
        for (int32_t i = 0; (i < (SNTP_RECV_TIMEOUT / 100)) && now.tv_sec < 1525952900; i++) {
            vTaskDelay(100 / portTICK_RATE_MS);
            gettimeofday(&now, NULL);
        }

        if (now.tv_sec < 1525952900) {
            sntp_retry_time = SNTP_RECV_TIMEOUT << sntp_retry_cnt;

            if (SNTP_RECV_TIMEOUT << (sntp_retry_cnt + 1) < SNTP_RETRY_TIMEOUT_MAX) {
                sntp_retry_cnt ++;
            }

            ESP_LOGI(TAG,"SNTP get time failed, retry after %d ms\n", sntp_retry_time);
            vTaskDelay(sntp_retry_time / portTICK_RATE_MS);
        } else {
            ESP_LOGI(TAG,"SNTP get time success\n");
            //在这里打印获取的时间
            break;
        }
    }
}
#endif

static void http_rest()
{
    char respond[1024]={0};
    memset(respond,0,	sizeof(respond));
	esp_http_client_config_t config = {
        .url = "https://api.seniverse.com/v3/weather/daily.json?key=SwcHTCpvgbfJC-sRX&location=beijing&language=zh-Hans&unit=c&start=0&days=5",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET
//    esp_err_t err = esp_http_client_perform(client);
//    if (err == ESP_OK) {
//        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
//                esp_http_client_get_status_code(client),
//                esp_http_client_get_content_length(client));
//    } else {
//        ESP_LOGI(TAG, "HTTP GET request failed: %d", err);
//    }
//    Cannot be called directly
//    esp_http_client_read(client, respond, sizeof(respond));
//    vTaskDelay(1000 / portTICK_RATE_MS);
//    ESP_LOGI(TAG,"strlen(respond) = %d",strlen(respond));
//    ESP_LOGI(TAG,"respond = %s",respond);
    // POST
//    const char *post_data = "field1=value1&field2=value2";
//    esp_http_client_set_url(client, "http://httpbin.org/post");
//    esp_http_client_set_method(client, HTTP_METHOD_POST);
//    esp_http_client_set_post_field(client, post_data, strlen(post_data));
//    err = esp_http_client_perform(client);
//    if (err == ESP_OK) {
//        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
//                esp_http_client_get_status_code(client),
//                esp_http_client_get_content_length(client));
//   		esp_http_client_read(client, respond, sizeof(respond));
//		ESP_LOGI(TAG,"respond = %s",respond);
//    } else {
//        ESP_LOGE(TAG, "HTTP POST request failed: %d", err);
//    }

//    //PUT
//    esp_http_client_set_url(client, "http://httpbin.org/put");
//    esp_http_client_set_method(client, HTTP_METHOD_PUT);
//    err = esp_http_client_perform(client);
//    if (err == ESP_OK) {
//        ESP_LOGI(TAG, "HTTP PUT Status = %d, content_length = %d",
//                esp_http_client_get_status_code(client),
//                esp_http_client_get_content_length(client));
//    } else {
//        ESP_LOGE(TAG, "HTTP PUT request failed: %d", err);
//    }
//
//    //PATCH
//    esp_http_client_set_url(client, "http://httpbin.org/patch");
//    esp_http_client_set_method(client, HTTP_METHOD_PATCH);
//    esp_http_client_set_post_field(client, NULL, 0);
//    err = esp_http_client_perform(client);
//    if (err == ESP_OK) {
//        ESP_LOGI(TAG, "HTTP PATCH Status = %d, content_length = %d",
//                esp_http_client_get_status_code(client),
//                esp_http_client_get_content_length(client));
//    } else {
//        ESP_LOGE(TAG, "HTTP PATCH request failed: %d", err);
//    }
//
//    //DELETE
//    esp_http_client_set_url(client, "http://httpbin.org/delete");
//    esp_http_client_set_method(client, HTTP_METHOD_DELETE);
//    err = esp_http_client_perform(client);
//    if (err == ESP_OK) {
//        ESP_LOGI(TAG, "HTTP DELETE Status = %d, content_length = %d",
//                esp_http_client_get_status_code(client),
//                esp_http_client_get_content_length(client));
//    } else {
//        ESP_LOGE(TAG, "HTTP DELETE request failed: %d", err);
//    }
//
//    //HEAD
//    esp_http_client_set_url(client, "http://httpbin.org/get");
//    esp_http_client_set_method(client, HTTP_METHOD_HEAD);
//    err = esp_http_client_perform(client);
//    if (err == ESP_OK) {
//        ESP_LOGI(TAG, "HTTP HEAD Status = %d, content_length = %d",
//                esp_http_client_get_status_code(client),
//                esp_http_client_get_content_length(client));
//    } else {
//        ESP_LOGE(TAG, "HTTP HEAD request failed: %d", err);
//    }
//
    esp_http_client_cleanup(client);
}

static void http_auth_basic()
{
    esp_http_client_config_t config = {
        .url = "http://user:passwd@httpbin.org/basic-auth/user/passwd",
        .event_handler = _http_event_handler,
        .auth_type = HTTP_AUTH_TYPE_BASIC,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP Basic Auth Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void http_auth_basic_redirect()
{
    esp_http_client_config_t config = {
        .url = "http://user:passwd@httpbin.org/basic-auth/user/passwd",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP Basic Auth redirect Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }

    esp_http_client_cleanup(client);
}

static void http_auth_digest()
{
    esp_http_client_config_t config = {
        .url = "http://user:passwd@httpbin.org/digest-auth/auth/user/passwd/MD5/never",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP Digest Auth Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void https()
{
    esp_http_client_config_t config = {
        .url = "https://www.howsmyssl.com",
        .event_handler = _http_event_handler,
        .cert_pem = howsmyssl_com_root_cert_pem_start,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void http_relative_redirect()
{
    esp_http_client_config_t config = {
        .url = "http://httpbin.org/relative-redirect/3",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP Relative path redirect Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void http_absolute_redirect()
{
    esp_http_client_config_t config = {
        .url = "http://httpbin.org/absolute-redirect/3",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP Absolute path redirect Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void http_redirect_to_https()
{
    esp_http_client_config_t config = {
        .url = "http://httpbin.org/redirect-to?url=https%3A%2F%2Fwww.howsmyssl.com",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP redirect to HTTPS Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}


static void http_download_chunk()
{
    esp_http_client_config_t config = {
        .url = "http://httpbin.org/stream-bytes/8912",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP chunk encoding Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void http_perform_as_stream_reader()
{
    char *buffer = malloc(MAX_HTTP_RECV_BUFFER + 1);
    if (buffer == NULL) {
        ESP_LOGE(TAG, "Cannot malloc http receive buffer");
        return;
    }
    esp_http_client_config_t config = {
        .url = "https://api.seniverse.com/v3/weather/daily.json?key=SwcHTCpvgbfJC-sRX&location=beijing&language=zh-Hans&unit=c&start=0&days=5",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err;
    if ((err = esp_http_client_open(client, 0)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %d", err);
        free(buffer);
        return;
    }
    int content_length =  esp_http_client_fetch_headers(client);
    int total_read_len = 0, read_len;
    if (total_read_len < content_length && content_length <= MAX_HTTP_RECV_BUFFER) {
        read_len = esp_http_client_read(client, buffer, content_length);
        if (read_len <= 0) {
            ESP_LOGE(TAG, "Error read data");
        }
        buffer[read_len] = 0;
        ESP_LOGD(TAG, "read_len = %d", read_len);
    }
    ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
    ESP_LOGI(TAG,"/n%s/n",buffer);
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    free(buffer);
}

static void https_async()
{
    esp_http_client_config_t config = {
        .url = "https://postman-echo.com/post",
        .event_handler = _http_event_handler,
        .is_async = true,
        .timeout_ms = 5000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err;
    const char *post_data = "Using a Palantír requires a person with great strength of will and wisdom. The Palantíri were meant to "
                            "be used by the Dúnedain to communicate throughout the Realms in Exile. During the War of the Ring, "
                            "the Palantíri were used by many individuals. Sauron used the Ithil-stone to take advantage of the users "
                            "of the other two stones, the Orthanc-stone and Anor-stone, but was also susceptible to deception himself.";
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    while (1) {
        err = esp_http_client_perform(client);
        if (err != ESP_ERR_HTTP_EAGAIN) {
            break;
        }
    }
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %d", err);
    }
    esp_http_client_cleanup(client);
}

static void http_test_task(void *pvParameters)
{
#if CONFIG_SSL_USING_WOLFSSL
    /* CA date verification need system time */
    get_time();
#endif

    app_wifi_wait_connected();
    ESP_LOGI(TAG, "Connected to AP, begin http example");
//
    //    http_rest();
    //Basic Authentication Scheme ，译为基本授权方案
//    http_auth_basic();
//    http_auth_basic_redirect();
//	http 服务器摘要认证
//    http_auth_digest();
//    http_relative_redirect();
//    http_absolute_redirect();
//    https();
//    HTTP重定向到HTTPS
//    http_redirect_to_https();
//    当客户端向服务器请求一个静态页面或者一张图片时，服务器可以很清楚的知道内容大小，
//    然后通过Content-Length消息首部字段告诉客户端需要接收多少数据。但是如果是动态页面等时，
//    服务器是不可能预先知道内容大小，这时就可以使用Transfer-Encoding：chunk模式来传输数据了。
//    即如果要一边产生数据，一边发给客户端
//    http_download_chunk();
    http_perform_as_stream_reader();
//    异步请求
//    https_async();
    ESP_LOGI(TAG, "Finish http example");
    vTaskDelete(NULL);
}

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    app_wifi_initialise();

    xTaskCreate(&http_test_task, "http_test_task", 8192, NULL, 5, NULL);
}
