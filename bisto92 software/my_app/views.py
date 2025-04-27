from django.shortcuts import render,redirect
from django.contrib.auth.models import auth
from .models import User,Booking,day,Feedback,Food,Order,Cart,Copn
from django.contrib import messages

from django.shortcuts import render,HttpResponse
from django.core.exceptions import ObjectDoesNotExist
from django.core.files.storage import default_storage
from django.shortcuts import get_object_or_404, render
from rest_framework import viewsets
from rest_framework.permissions import (IsAdminUser, IsAuthenticated,
                                        IsAuthenticatedOrReadOnly)
from rest_framework.views import APIView
from .serializers import *
from rest_framework import status
from rest_framework.response import Response

class OrderAPIView(APIView):


    permission_classes = []
    serializer_class = OrderSerializer


    def post(self,request):
        """

        """
        data = request.data

        required_field = []

        for i in required_field:
            if i not in data:
                return Response({"msg":"Please Provide a valid action."},status=status.HTTP_406_NOT_ACCEPTABLE)

        
        # products = data["products"]

        print(data['Items'])
        print(data['Table'])

        s = ''
        for i in data['Items']:
            s+=f' {i["Quantity"]}'
            s+=f' {i["Food"]}'
            s+=' And'
        
        s = s[:-3]
        print(s)


        if "Status" in data:
            pass

        else:
            return Response({"msg":"Please Provide a valid action."},status=status.HTTP_406_NOT_ACCEPTABLE)

        order_table = data['Table']
        print(order_table)
        try:

            order = Order.objects.create(food_list = s, order_table = int(order_table))
        except:
            print("Not Created The product ")

        return Response({"msg":"Please Provide a valid action."},status=status.HTTP_406_NOT_ACCEPTABLE)
    


def home(request):
    return render(request,'index.html')


def login(request):
    if request.method =='GET':
        return render(request,'login_register.html')
    else:
        email = request.POST['email']
        password = request.POST['pswd']
        user = auth.authenticate(email = email ,password = password)
        if user is not None:
            auth.login(request,user)
            return redirect("/")
        else:
            messages.error(request,"User Not found.")
            return redirect('/login')

def register(request):
    if request.method=='GET':
        if request.user.is_authenticated:
            return redirect('/')
        else:
            return render(request,'login_register.html')
    else:       
        email = request.POST['email']
        if User.objects.filter(email=email).exists():
            messages.error(request,"Email Already Exist. Please Login")
            return redirect('/register')
        name = request.POST['name']
        password = request.POST['pswd']
        user = User.objects.create_user(email,password)
        user.f_name = name
        user.save()
        userr = auth.authenticate(email = email ,password = password)
        if userr is not None:
            auth.login(request,userr)
            return redirect("/")
        else:
            messages.error(request,"User Not found.")
            return redirect('/login')

def logout(request):
    auth.logout(request)
    return redirect('/')

def booking(request):
    if request.method=='GET':

        if request.user.is_authenticated:
            d = day.objects.latest('id')
            return render(request,'booking.html',{"data":d})
        else:
            return redirect('/login')
    else:
        fname = request.POST['fname']
        phone = request.POST['phone']
        time =  request.POST['time']
        setcount = request.POST['setcount']
        payment_method = request.POST['p_m']
        cus = Booking(owner=request.user,full_name = fname,phone_number = phone,no_of_seat=setcount,payment_method= payment_method,time = time )
        d = day.objects.latest('id')
        x = getattr(d, time)  
        if x<=0:
            messages.error(request," Sorry Sir , No seat avaliable For this Time")
            return redirect('/booking')

        print(x)
        cus.save()
        return redirect('/make_payment/'+str(cus.id))
def feedback(request):
    if request.method == 'POST':
        name = request.POST['name']
        email = request.POST['email']
        subject = request.POST['subject']
        message = request.POST['message']
        feed = Feedback(name = name,email = email,subject = subject,message = message)
        feed.save()
        return render(request,'thank_you.html',{"m":"Thanks For Your Feedback "})
def make_payment(request,pk):
    if request.method == 'GET':
        order = Booking.objects.get(id=pk)
        taka = order.no_of_seat *100
        if order.payment_method =='bkash':
            return render(request,'payment.html',{"data":order,"taka":taka})
        elif order.payment_method == 'nagad':
            return render(request,'payment_with_nagad.html',{"data":order,"taka":taka})
        else:
            d = day.objects.latest('id')
            x = getattr(d, order.time)
            setattr(d, order.time, x-int(order.no_of_seat))
            d.save()
            order.save()
            return render(request,'thank_you.html',{"m":"Successfully completed"})

    if request.method == 'POST':
        order = Booking.objects.get(id=pk)
        tid = request.POST['tid']
        d = day.objects.latest('id')
        x = getattr(d, order.time)
        setattr(d, order.time, x-int(order.no_of_seat))
        d.save()
        order.t_id = tid
        order.save()
        return render(request,'thank_you.html',{"m":"Successfully completed"})

def make_order(request,food_id):
    if request.method == 'GET':
        food = Food.objects.get(food_id=food_id)
        return render(request,'payment2.html',{"data": food})
    if request.method == 'POST':
        food = Food.objects.get(food_id=food_id)
        itemid = food_id
        order_table = request.POST['order_table']
        tid = request.POST['tid']
     
        order = Order(owner = request.user,itemid = itemid,order_table=order_table,tid = tid)
        order.save()
        return render(request,'thank_you.html',{"m":"Order Successfully Received"})





def make_order_nagad(request,food_id):
    if request.method == 'GET':
        food = Food.objects.get(food_id=food_id)
        return render(request,'payment2_with_nagad.html',{"data": food})

def show_data(request,what):
    if request.user.is_superuser:
        booked = Booking.objects.all().order_by('-id')
        order = Order.objects.filter(is_food_served = False)
        if what=='order':
            return render(request,'All_Booking_order.html',{"data":order,"isorder":True})
        elif what == 'booking':
            return render(request,'All_Booking_order.html',{"data":booked})
        else:
            return redirect('/')
    else:
        return render(request,'thank_you.html',{"m":"You Are Not Allowed To Show All Order Or Booking"})

def del_order(request,pk):
    if request.user.is_superuser:
        order = Order.objects.get(id=int(pk))
        order.delete()
        return redirect('/show/order')
def apc_order(request,pk):
    if request.user.is_superuser:
        order = Order.objects.get(id=int(pk))
        order.is_payment_done = True
        # food = Food.objects.get(food_id = order.itemid)
        # d = day.objects.latest('id')
        # d.total_sell+= food.price
        # if food.stock !=None:
        #     food.stock-=1
        #     food.save()
        order.save()
        # d.save()
        return redirect('/show/order')

def add_to_cart(request,food_id):
    if request.user.is_authenticated:
        food = Food.objects.get(food_id = food_id)
        newitem = Cart(owner = request.user,food_id=food)
        newitem.save()
        return redirect('/')
    else:
        return redirect('/login')



def show_cart(request):
    if request.user.is_authenticated:
        cart = Cart.objects.filter(owner = request.user)
        total = 0
        for i in cart:
            total+=(i.food_id.price*i.quantity)
        Total = total+20
        
       
        return render(request,'show_cart.html',{"data":cart,"sub_total":total,"Total":Total})

def get_cart_order(request):
    d = day.objects.latest('id')
    cart = Cart.objects.filter(owner = request.user)
    for i in cart:
        d.total_sell+=i.food_id.price
        s = Food.objects.get(food_id = i.food_id.food_id)
        s.stock-=i.quantity
        s.save()
        i.delete()
    d.total_sell+=20
    d.save()
    return render(request,'thank_you.html',{"m":"Your Order Accepted"})

def get_cart_order_table(request):
    table = request.POST['table']
    promo = request.POST['promo_code']
    flag = False
    discount = 0
    if promo != "":
        cpn = Copn.objects.all()
        for j in cpn:
            if j.cpn_code == promo:
                flag = True
                discount = j.discount
                break
    d = day.objects.latest('id')
    cart = Cart.objects.filter(owner = request.user)
    foodname = ""
    for i in cart:
        foodname+=i.food_id.name
        foodname+=";"
        d.total_sell+=i.food_id.price
        s = Food.objects.get(food_id = i.food_id.food_id)
        x = s.stock
        y = i.quantity
        s.stock= (x-y)
        s.save()
        i.delete()
        
    d.total_sell+=20
    if flag:
        d.total_sell-=discount
    d.save()
    order = Order(owner = request.user,itemid = "null",order_table=table,tid = "Card payment",food_list = foodname)
    order.save()
    if flag:
        return render(request,'thank_you.html',{"m":"You Received Discount "+ str(discount)+" Taka Accepted"})
    else:
        return render(request,'thank_you.html',{"m":"Your Order Apcepted"})



def food_ready(request,pk):
    if request.user.is_superuser:

        order = Order.objects.get(id = int(pk))
        order.is_food_ready = True
        order.save()
        return redirect('/show/order')
    else:
        return render(request,'thank_you.html',{"m":"You are not admin"})
def food_served(request,pk):
    if request.user.is_superuser:

        order = Order.objects.get(id = int(pk))
        order.is_food_served = True
        order.save()
        return redirect('/show/order')
    else:
        return render(request,'thank_you.html',{"m":"You are not admin"})

def display(request):
    order = Order.objects.filter(is_food_served = False)
    return render(request,'display.html',{"data":order})


def add_many_cart(request,pk):
    if request.user.is_authenticated:
        cart = Cart.objects.get(id = pk)
        cart.quantity+=1
        cart.save()
        return redirect('/show_cart')
    else:
        return redirect('/login')
        
def remove_from_cart(request,pk):
    if request.user.is_authenticated:
        cart = Cart.objects.get(id = pk)
        if cart.quantity==1:
            cart.delete()
            return redirect('/show_cart')
        cart.quantity-=1
        cart.save()
        return redirect('/show_cart')
    else:
        return redirect('/login')

def calc(mo):
    m1 = day.objects.filter(date__month=mo)
    pm1 = 0
    for i in m1:
        pm1+=i.total_sell
    return pm1

def show_admin(request):
    if request.user.is_superuser:
        food = Food.objects.all().exclude(food_id="null")
        
        return render(request,'admin.html',{"data":food,"m1":calc('01'),"m2":calc('02'),"m2":calc('02'),"m3":calc('03'),"m4":calc('04'),"m5":calc('05'),"m6":calc('06'),"m7":calc('07'),"m8":calc('08'),"m9":calc('09'),"m10":calc('10'),"m11":calc('11'),"m12":calc('12')})
    else:
        return render(request,'thank_you.html',{"m":"Sorry You Are Not Admin"})

    

