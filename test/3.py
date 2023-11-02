#1.打印句子
def  display_message():
    print("this is python")
    
display_message()


#2.书名
def favorite_book(title):
    print("One of my favorite books is "+title.title()+".")
favorite_book('python')

#3.T恤
def make_shirt(size, picture):
    print("The T-shirt's size is " + size + ", and the picture is " + picture + ".")

make_shirt('xxl', 'welcome')

#4.大号T恤
def make_shirt(size, picture='I love Python'):
    print("Size is " + size + ", and picture is " + picture + ".")

make_shirt('L')
make_shirt('XL')
make_shirt('XXL', 'welcome')

#5.describe_city() 
def describe_city(city,pro='中国'):
    
    print("\n"+city + "属于" + pro + ".")
describe_city('江苏')
describe_city(city='黑龙江')
describe_city(city='东京',pro='日本')


#6. city_country()
def city_country(city_name,country_name):
    print(city_name.title()+','+country_name.title())
city_country('Beijing','china')
city_country('shanghai','china')
city_country('shenyang','china')



