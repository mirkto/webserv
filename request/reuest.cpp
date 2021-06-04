/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reuest.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monie <monie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 15:15:16 by monie             #+#    #+#             */
/*   Updated: 2021/05/30 19:47:17 by monie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reuest.hpp"

void Reuest::reuest_init() {
	index = 0;
	i_end = 0;
	len = 0;
	f_error = 0;
	buf.clear();
	_start_line._f_sl_status = 0;
	_headers._f_hd_status = 0;
	_body._f_bd_status = 0;
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
	int j = buf.find("\r\n");
	for(int i = 0; i < len; i++) {
		if(buf[i] == ' ' && flag == 0) {
			_start_line._type_reuest = word;
			word.clear();
			flag = 1;
			continue;
		} else if (buf[i] == ' ' && flag == 1) {
			_start_line._resource_name = word;
			word.clear();
			flag = 2;
			continue;
		} else if (i < j) {
			_start_line._protocol_version = word;
			word.clear();
			break;
		} else if (buf[i] == ' ' && flag == 2) {
			f_error = 1;
			std::cout << "Error!" << std::endl;
		} 
		word += buf[i];
	}
	_start_line._f_sl_status = 1;
}

void Reuest::filling_map(std::string &word) {
	std::string key;
	std::string value;
	int flag = 0;
	for(int i = 0; word[i] != '\0'; i++) {
		if(word[i] == ':' && word[i + 1] == ' ') {
			flag = 1;
			i++;
			continue;
		} else if(flag == 0) {
			key += word[i];
		} else {
			value += word[i];
		}
	}
	if(_headers.mp.count(key) == 0) {
		_headers.mp.insert(std::pair<std::string, std::string>(key, value));	
	}
}

void Reuest::filling_headers() {
	std::string word;
	int flag = 0;
	int j = buf.find("\r\n\r\n");
	for (int i = 0; i < j; i++) {
		if(buf[i] == '\r' && buf[i + 1] == '\n') {
			filling_map(word);
			word.clear();
		}
		word += buf[i];
	}
	_headers._f_hd_status = 1;
}

void Reuest::filling_body() {
	_start_line._f_sl_status = 1;
}

void Reuest::func_reuest(std::string str) {
	buf += str;
	len = buf.length();
	if(_start_line._f_sl_status == 0 && find_end("\r\n", 2)) {
		//std::cout << "st -- " << buf << std::endl;
		filling_start_line();
		new_buf();
	} else if(_start_line._f_sl_status == 1 && _headers._f_hd_status == 0 &&\
		find_end("\r\n\r\n", 4)) {
		std::cout << "hd -- " << buf << std::endl;
		filling_headers();
		new_buf();
	} else if(_start_line._f_sl_status == 1 && _headers._f_hd_status == 1 &&\
		_body._f_bd_status == 0) {
		std::cout << "bd -- " << std::endl;
		filling_body();
		new_buf();
	} else {
		//std::cout << "Not end! " << buf << std::endl;
	}
	if(_start_line._f_sl_status == 1 && _headers._f_hd_status == 1 &&\
		_body._f_bd_status == 1) {
			reuest_init();
	}
}

int main() {
	Reuest reuest;
	reuest.reuest_init();
	reuest.func_reuest("GET /images/corne");
	reuest.func_reuest("r1.png HTTP/1.1\r\nHost: mail.example.com\r\n");
	reuest.func_reuest("Referer: http://mail.example.com/send-message.html\r\n");
	reuest.func_reuest("User-Agent: BrowserForDummies/4.67b\r\n");
	reuest.func_reuest("Content-Type: multipart/form-data; boundary=\"Asrf456BGe4h\"\r\nContent-Length: 170\r\n");
	reuest.func_reuest("Connection: keep-alive\r\n");
	reuest.func_reuest("Keep-Alive: 300\r\n");
	reuest.func_reuest("\r\n\r\nПротокол HTTP поддерживает передачу нескольких сущностей в пределах одного сообщения. Причём сущности могут передаваться не только в виде одноуровневой последовательности, но и в виде");
	return 0;
}