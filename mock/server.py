from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import urllib

response = {
    "flg": 1,
    "tim": "20240924151206",
    "tmp": 345,
    "fac": 4998,
    "pac": 716,
    "sac": 716,
    "qac": 0,
    "eto": 19627,
    "etd": 294,
    "hto": 745,
    "pf": 75,
    "wan": 0,
    "err": 0,
    "vac": [2366, 2405, 2387],
    "iac": [13, 13, 13],
    "vpv": [3634, 3567],
    "ipv": [103, 84],
    "str": [],
}
response = json.dumps(response)


class MockServerRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        print("GET request received from {}".format(self.client_address))

        parsed_path = urllib.parse.urlparse(self.path)
        query_params = urllib.parse.parse_qs(parsed_path.query)

        if (
            parsed_path.path == "/getdevdata.cgi"
            and query_params.get("device") == ["2"]
            and query_params.get("sn") == ["SZ00126022270436"]
        ):

            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.send_header("Content-Length", len(response))
            self.end_headers()
            self.wfile.write(response.encode("utf-8"))
        else:
            print("Invalid request")
            self.send_response(404)
            self.end_headers()


def run():
    server_address = ("", 8484)
    httpd = HTTPServer(server_address, MockServerRequestHandler)
    print("Running mock server...")
    httpd.serve_forever()


if __name__ == "__main__":
    run()
