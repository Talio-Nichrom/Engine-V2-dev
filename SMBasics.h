#pragma once

#ifndef SM_BASICS
#define SM_BASICS

#include <Windows.h>	//������ � Windows
#include <string>		//������
#include <algorithm>	//��������� ���������, � �.�. �� ��������
#include <memory>		//����� ���������
#include <map>			//�������
#include <vector>		//������
#include <functional>	//�������
#include <tuple>		//������������ �����
#include <locale>		//�������������� ������������ ��������� � unicode
#include <tchar.h>		//��������� ������������ ��������� � unicode


//������� ������������ ����, ������������ �� ���� ������
//��������� ���, ��� ���������� ��� �������� �� ���������.
//�������� ��� ����������� ������� ���������� � ������� ����������.

//������� ������������ ���������� � ����������� � ����������
#define s_release(p) { if(p) { (p)->Release(); (p) = 0; } }
#define s_deleteP(p) { if(p) { delete (p); (p) = 0; } }
#define s_deleteA(p) { if(p) { delete[] (p); (p) = 0; } }

//������ ���������
namespace sm
{
	//��������� ���������� ��� ��������
	template < typename... Args >
	class IDelegateContainer
	{
		public:
		virtual ~IDelegateContainer()
		{
		}
		virtual void call( Args... args ) = 0;

	};

	//��������� ��������� ��� ��������
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

	//������� � ��������� ������� ���������� � ������� ������������� ������
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

	//���������� ������� ���������
	typedef Delegate<> VoidDelegate; //void ()

	//� ������ ������� ������ �������� ���������
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