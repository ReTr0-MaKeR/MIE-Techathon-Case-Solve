"""my_project URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from . import views

urlpatterns = [
    path('', views.home),
    path('login', views.login),
    path('register', views.register),
    path('logout', views.logout),
    path('booking', views.booking),
    path('feedback', views.feedback),
    path('make_payment/<int:pk>', views.make_payment),
    path('make_order/<str:food_id>', views.make_order),
    path('add_to_cart/<str:food_id>', views.add_to_cart),
    path('make_order_with_nagad/<str:food_id>', views.make_order_nagad),
    path('show/<str:what>', views.show_data),
    path('delete_order/<str:pk>', views.del_order),
    path('apc_order/<str:pk>', views.apc_order),
    path('show_cart', views.show_cart),
    path('get_cart_order', views.get_cart_order),
    path('food_ready/<int:pk>', views.food_ready),
    path('food_served/<int:pk>', views.food_served),
    path('display',views.display),
    path('multiple_order',views.get_cart_order_table),
    path('add/<int:pk>',views.add_many_cart),
    path('remove/<int:pk>',views.remove_from_cart),
    path('show_admin',views.show_admin),
    path('order/create_order/',views.OrderAPIView.as_view(),name="hello")
]
