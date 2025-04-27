from django.db import models
from .manager import UserManager
from django.contrib.auth.models import AbstractUser
class User(AbstractUser):
    username = None
    email = models.EmailField(unique=True)
    phone = models.CharField(max_length=15)
    f_name = models.CharField(max_length=100,default="roy")

    objects = UserManager()
    USERNAME_FIELD = 'email'
    REQUIRED_FIELDS= []

class day(models.Model):
    total_sell = models.IntegerField(default=0)
    date = models.DateField(auto_now_add=True)
    t10t11 = models.IntegerField(default=20)
    t11t12 = models.IntegerField(default=20)
    t12t1 = models.IntegerField(default=20)
    t1t2 = models.IntegerField(default=20)
    t2t3 = models.IntegerField(default=20)
    t3t4 = models.IntegerField(default=20)
    t4t5 = models.IntegerField(default=20)
    t5t6 = models.IntegerField(default=20)
    t6t7 = models.IntegerField(default=20)
    t7t8 = models.IntegerField(default=20)
    t8t9 = models.IntegerField(default=20)
    t9t10 = models.IntegerField(default=20)
    def __str__(self):
        return str(self.date)

class Booking(models.Model):
    owner = models.ForeignKey(User,on_delete=models.CASCADE,related_name="owner")
    full_name = models.CharField(max_length=100,blank=True,null=True)
    phone_number = models.CharField(max_length=15,blank=True,null=True)
    set_no = models.IntegerField(null=True,blank=True)
    no_of_seat = models.IntegerField(default=0)
    t_id = models.CharField(max_length=100)
    payment_method = models.CharField(max_length=10,null=True,blank=True)
    time = models.CharField(max_length=100)

class Feedback(models.Model):
    name = models.CharField(max_length=100)
    email = models.EmailField()
    subject = models.CharField(null=True,blank=True , max_length=100)
    message = models.TextField(max_length=500)
    def __str__(self):
        return self.email
class Order(models.Model):
    owner = models.ForeignKey(User,on_delete=models.CASCADE,related_name="order_owner",null=True,blank=True)
    itemid = models.CharField(max_length=15,null=True,blank=True)
    order_table = models.IntegerField()
    tid = models.CharField(max_length=10,null=True,blank=True)
    payment_method = models.CharField(max_length=10,null=True,blank=True)
    is_payment_done = models.BooleanField(default=False)
    is_food_ready = models.BooleanField(default=False)
    is_food_served = models.BooleanField(default=False)
    date_time = models.DateTimeField(auto_now_add=True)
    food_list = models.CharField(null=True,blank=True,max_length=1000)
    cpn_code = models.CharField(null=True,blank=True,max_length=15)

class Copn(models.Model):
    cpn_code = models.CharField(max_length=15)
    discount = models.IntegerField()



class Food(models.Model):
    food_id = models.CharField(max_length=20)
    name = models.CharField(max_length=100)
    price = models.IntegerField()
    stock = models.IntegerField(null=True,blank=True)
    modifyed = models.DateTimeField(auto_now=True)
    def __str__(self):
        return self.name

class Cart(models.Model):
    owner = models.ForeignKey(User,on_delete=models.CASCADE,related_name="cart_owner")
    food_id = models.ForeignKey(Food,on_delete=models.CASCADE,related_name="food_details")
    table_no = models.IntegerField(null=True,blank=True)
    quantity = models.IntegerField(default=1)


    
