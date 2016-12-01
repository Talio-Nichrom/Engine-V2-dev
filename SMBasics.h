#pragma once

#ifndef SM_BASICS
#define SM_BASICS

#include <Windows.h>	//Работа с Windows
#include <string>		//Строки
#include <algorithm>	//Различные алгоритмы, в т.ч. со строками
#include <memory>		//Умные указатели
#include <map>			//Словари
#include <vector>		//Списки
#include <functional>	//Функции
#include <tuple>		//Произвольный набор
#include <locale>		//Преобразование многобайтной кодировки в unicode
#include <tchar.h>		//Обобщение многобайтной кодировки и unicode


//Базовый заголовочный файл, используемый во всех частях
//программы так, что подключать его отдельно не требуется.
//Содержит все необходимые базовые интерфейсы и макросы управления.

//Макросы освобождения указателей и интерфейсов с обнулением
#define s_release(p) { if(p) { (p)->Release(); (p) = 0; } }
#define s_deleteP(p) { if(p) { delete (p); (p) = 0; } }
#define s_deleteA(p) { if(p) { delete[] (p); (p) = 0; } }

//Классы делегатов
namespace sm
{
	//Интерфейс контейнера для делегата
	template < typename... Args >
	class IDelegateContainer
	{
		public:
		virtual ~IDelegateContainer()
		{
		}
		virtual void call( Args... args ) = 0;

	};

	//Шаблонный контейнер для делегата
	template< class T, typename... Args > class ClassDelegateContainer : public IDelegateContainer< Args... >
	{
		typedef void ( T::*LPMethod )( Args... args );

		public:
			ClassDelegateContainer( T* pClass, LPMethod pMethod ) :
				mpClass( pClass ),
				mpMethod( pMethod )
			{
			}

			void call( Args... args )
			{
				if( mpClass && mpMethod )
					( mpClass->*mpMethod )( std::forward<Args>( args )... );
			}

		private:
			T* mpClass;
			LPMethod mpMethod;
	};

	template< typename... Args > class DelegateContainer : public IDelegateContainer< Args... >
	{
		typedef void ( *LPMethod )( Args... args );

		public:
			DelegateContainer( LPMethod pMethod ) :
				mpMethod( pMethod )
			{
			}

			void call( Args... args )
			{
				if( mpMethod )
					( *mpMtehod )( std::forward< Args >( args )... );
			}

		private:
			LPMethod mpMethod;
	};

	//Делегат с шаблонным методом соединения с методом произвольного класса
	template < typename... Args>
	class Delegate
	{
		public:
			Delegate()
			{
			}

			~Delegate()
			{
			}

			template< class T >
			void connect( T* pClass, void ( T::*pMethod )( Args... args ) )
			{
				IDelegateContainer< Args... > *pDelegateContainer = new ClassDelegateContainer< T, Args... >( pClass, pMethod );
				mpDelegateContainer = std::shared_ptr< IDelegateContainer< Args... > >( pDelegateContainer );
			}

			void connect( void ( *pMethod )( Args... args ) )
			{
				IDelegateContainer< Args... > *pDelegateContainer = new DelegateContainer< Args... >( pMethod );
				mpDelegateContainer = std::shared_ptr< IDelegateContainer< Args... > >( pDelegateContainer );
			}

			void operator()( Args... args )
			{
				if( mpDelegateContainer )
					mpDelegateContainer->call( std::forward< Args >( args )... );
			}

		private:
			std::shared_ptr< IDelegateContainer< Args...> > mpDelegateContainer;
	};

	//Объявление простых делегатов
	typedef Delegate<> VoidDelegate; //void ()

	//В рамках проекта должно работать нормально
	#ifdef _UNICODE
		typedef std::wstring tstring;
		#define stringToTstring( x )	tstring( x.begin(), x.end() )
		#define toTstring( x )	std::to_wstring( x )
	#else
		typedef std::string tstring;
		#define stringToTstring( x 	tstring( x.begin(), x.end() )
		#define toTstring( x )	std::to_string( x )
	#endif

}

#endif