/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reuest.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monie <monie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 16:14:24 by monie             #+#    #+#             */
/*   Updated: 2021/05/30 18:02:33 by monie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iostream"
#include "map"

struct Start_line
{
	std::string	_type_reuest;
	std::string	_resource_name;
	std::string	_protocol_version;
	int			_f_sl_status;
};

struct Headers
{
	std::map <std::string, std::string> mp;
	int	_f_hd_status;
};

struct Body
{
	int	_f_bd_status;
};

struct Reuest {
	std::string buf;
	int index;
	int i_end;
	int len;
	int f_error;
	Start_line _start_line;
	Headers _headers;
	Body _body;
	int find_end(std::string end, int i);
	void new_buf();
	void filling_start_line();
	void filling_map(std::string &word);
	void filling_headers();
	void filling_body();
	void reuest_init();
	void func_find_end();
	void func_reuest(std::string str);
};