#include "reuest.hpp"

int Reuest::str_to_un_int(std::string str) {
	int ui = 0;
	int len = str.length();
	for(int i = 0; i < len; i++) {
		if(i != 0) {
			ui *= 10;
		}
		ui += str[i] - '0';
	}
	return ui;
}

int Reuest::hex_to_dec(std::string st)
{
	int i, k;
	unsigned int s = 0;
	for (i = 0; st[i] != '\0'; i++)
	{
		int c;
		switch (c = toupper(st[i]))
		{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			k = c - 'A' + 10;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			k = c - '0';
			break;
		}
		s = (s << 4) + k;
	}
	return s;
}

void Reuest::reuest_init() {
	index = 0;
	i_end = 0;
	len = 0;
	f_error = 0;
	buf.clear();
	_f_sl_status = 0;
	_f_hd_status = 0;
	_f_bd_status = 0;
	_f_end_reuest = 0;
	method_body = 0;
}

int Reuest::find_end(std::string end, int i) {
	int j = buf.find(end);
	if( j != -1) {
		i_end = i + j;
		return 1;
	}
	return 0;
}

void Reuest::new_buf() {
	std::string tmp_buf;
	for(;i_end < len; i_end++) {
		tmp_buf += buf[i_end];
	}
	buf.clear();
	buf = tmp_buf;
	index = 0;
	i_end = 0;
}

void Reuest::filling_start_line() {
	std::string word;
	int flag = 0;
	for(int i = 0; i < len; i++) {
		if(buf[i] == ' ' && flag == 0) {
			_type_reuest = word;
			word.clear();
			flag = 1;
			continue;
		} else if (buf[i] == ' ' && flag == 1) {
			_resource_name = word;
			word.clear();
			flag = 2;
			continue;
		} else if (buf[i] == '\r' && buf[i + 1] == '\n' && flag == 2) {
			_protocol_version = word;
			word.clear();
			flag = 3;
			break;
		} 
		word += buf[i];
	}
	_f_sl_status = 1;
}

void Reuest::filling_headers() {
	std::string word;
	std::string key;
	int flag = 0;
	for(int i = 0; i < len; i++) {
		if(buf[i] == ':' && buf[i + 1] == ' ' && flag == 0) {
			i += 2;
			key = word;
			word.clear();
			flag = 1;
		} else if (buf[i] == '\r' && buf[i + 1] == '\n' && flag == 1) {
			i += 2;
			headers.insert(make_pair(key, word));
			word.clear();
			flag = 0;
		}
		word += buf[i];
	}
	if(headers.count("Content-Length")) {
		method_body = str_to_un_int(headers["Content-Length"]);
	}
	if(headers.count("Transfer-Encoding")) {
		method_body = -1;
	}
	_f_hd_status = 1;
}

void Reuest::filling_body_length() {
	int i = 0;
	std::string tmp;
	for(; i < method_body; i++) {
		_body += buf[i];
	}
	for (; i < len; i++) {
		tmp += buf[i];
	}
	buf.clear();
	buf = tmp;
	_f_bd_status = 1;
}

void Reuest::filling_body_encoding() {
	std::string tmp;
	int cache = -1;
	int i = 0;
	for(; i < len; i++) {
		if(buf[i] == '\r' && buf[i + 1] == '\n' && cache == -1) {
			cache = hex_to_dec(tmp);
			tmp.clear();
			i += 2;
		} else if (cache != -1 && buf[i] == '\r' && buf[i + 1] == '\n') {
			if(tmp.length() == cache) {
				std::cout << "ok" << std::endl;
			} else {
				
				std::cout << "not ok" << tmp.length() << std::endl;
			}
			_body += tmp;
			tmp.clear();
			cache = -1;
			i += 2;
		}
		tmp += buf[i];
	}
}

void Reuest::func_reuest(std::string str) {
	buf += str;
	len = buf.length();
	if(_f_sl_status == 0 && find_end("\r\n", 2)) {
		filling_start_line();
		new_buf();
	} else if(_f_sl_status == 1 && _f_hd_status == 0 &&\
		find_end("\r\n\r\n", 4)) {
		filling_headers();
		new_buf();
	} else if(_f_sl_status == 1 && _f_hd_status == 1 &&\
		_f_bd_status == 0 && method_body > 0) {
		filling_body_length();
		new_buf();
	} else if(_f_sl_status == 1 && _f_hd_status == 1 &&\
		_f_bd_status == 0 && method_body == -1 && find_end("\r\n0\r\n", 5)) {	
		filling_body_encoding();
		new_buf();
	}
}

void Reuest::see_reuest() {
	std::cout << "------------------------------------------------------------------------" << std::endl;
	std::cout << "-------------------------------start line-------------------------------" << std::endl;
	std::cout << "Type:\t\t" << _type_reuest << std::endl;
	std::cout << "Resource:\t" << _resource_name << std::endl;
	std::cout << "Version:\t" << _protocol_version << std::endl;
	std::cout << "-------------------------------headers----------------------------------" << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++) {
		if((it->first).length() < 8) 
			std::cout << "key\t" << it->first << "\t\t\tvalue\t\t" << it->second << std::endl;
		else if((it->first).length() < 15)
			std::cout << "key\t" << it->first << "\t\tvalue\t\t" << it->second << std::endl;
		else
			std::cout << "key\t" << it->first << "\tvalue\t\t" << it->second << std::endl;
	}
	std::cout << "-------------------------------body-------------------------------------" << std::endl;
	std::cout << _body << std::endl;
	std::cout << "-------------------------------left-------------------------------------" << std::endl;
	std::cout << buf << std::endl;
	std::cout << "-------------------------------end--------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------------------------" << std::endl;
}

int main() {
	Reuest reuest;
	reuest.reuest_init();
	reuest.func_reuest("GET /images/corne");
	reuest.func_reuest("r1.png HTTP/1.1\r\nHost: mail.example.com\r\n");
	reuest.func_reuest("Referer: http://mail.example.com/send-message.html\r\n");
	reuest.func_reuest("User-Agent: BrowserForDummies/4.67b\r\n");
	reuest.func_reuest("Content-Type: multipart/form-data; boundary=\"Asrf456BGe4h\"\r\nConnection: keep-alive\r\n");
	reuest.func_reuest("Transfer-Encoding: chunked\r\n");
	//reuest.func_reuest("Content-Length: 226\r\n");
	reuest.func_reuest("Keep-Alive: 300\r\n");
	reuest.func_reuest("\r\n64\r\nLinux семейство Unix-подобных операционных систем на базе ядр");
	reuest.func_reuest("а Linux, включающих тот или иной  набор\r\n64\r\nLinux семейство Unix-подобных операционн");
	reuest.func_reuest("ых систем на базе ядр а Linux, включающих тот или иной  набор\r\n0\r\nafafaffa");
	reuest.see_reuest();
	return 0;
}