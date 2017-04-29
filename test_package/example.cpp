/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <injeqt/injector.h>
#include <injeqt/module.h>

#include <QtCore/QObject>
#include <iostream>
#include <memory>
#include <string>

class hello_service : public QObject
{
	Q_OBJECT

public:
	hello_service() {}
	virtual ~hello_service() {}

	std::string say_hello() const
	{
		return {"Hello"};
	}
};

class world_service : public QObject
{
	Q_OBJECT

public:
	world_service() {}
	virtual ~world_service() {}

	std::string say_world() const
	{
		return {"World"};
	}
};

class hello_factory : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE hello_factory() {}
	virtual ~hello_factory() {}

	Q_INVOKABLE hello_service * create_service()
	{
		return new hello_service{};
	}
};

class hello_client : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE hello_client() : _s{nullptr}, _w{nullptr} {}
	virtual ~hello_client() {}

	std::string say() const
	{
		return _s->say_hello() + " " + _w->say_world() + "!";
	}

private slots:
	INJEQT_INIT void init()
	{
		std::cerr << "all services set" << std::endl;
	}

	INJEQT_DONE void done()
	{
		std::cerr << "ready for destruction" << std::endl;
	}

	INJEQT_SET void set_hello_service(hello_service *s)
	{
		_s = s;
	}

	INJEQT_SET void set_world_service(world_service *w)
	{
		_w = w;
	}

private:
	hello_service *_s;
	world_service *_w;

};

class module : public injeqt::module
{
public:
	explicit module()
	{
		_w = std::unique_ptr<world_service>{new world_service{}};

		add_type<hello_client>();
		add_type<hello_factory>();
		add_factory<hello_service, hello_factory>();
		add_ready_object<world_service>(_w.get());
	}

	virtual ~module() {}

private:
	std::unique_ptr<world_service> _w;

};

int main()
{
	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<injeqt::module>{new module{}});

	auto injector = injeqt::injector{std::move(modules)};
	auto client = injector.get<hello_client>();
	auto hello = client->say();

	std::cout << hello << std::endl;
}

#include "example.moc"
