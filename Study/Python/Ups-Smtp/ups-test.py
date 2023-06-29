import os
import smtplib
from email.mime.text import MIMEText
from datetime import datetime

def send_email(subject, message, recipient):
    sender = "xxx@163.com" # 替换为发件人邮箱地址
    password = "" # 替换为发件人邮箱密码

    # 设置邮件内容
    email = MIMEText(message)
    email["Subject"] = subject
    email["From"] = sender
    email["To"] = recipient

    # 连接SMTP服务器
    with smtplib.SMTP_SSL("smtp.qq.com", 465) as server: # 替换为发件人邮箱SMTP服务器地址
        # 登录SMTP服务器
        server.login(sender, password)

        # 发送邮件
        server.sendmail(sender, [recipient], email.as_string())

        print("邮件发送成功！")

def main():
    command = "baidu.com"  # 替换为要ping的域名或IP地址
    recipient = ""  # 替换为要发送邮件的邮箱地址
    subject = "UPS输入市电恢复"  # 替换为邮件主题

    # 动态生成DateTime内容
    now = datetime.now()
    datetime_str = now.strftime("%Y-%m-%d %H:%M:%S")
    message = f"""
    来自 LADIS UPS 的 事件提醒
    事件: UPS输入市电异常；
    类型: 输入事件;
    Contact: 你的邮箱地址;
    DateTime: {datetime_str}
    """  # 替换为要发送的邮件内容

    ping_result = 1
    while ping_result != 0:
        # 执行ping命令
        ping_result = os.system(f"ping -c 1 {command}")

        if ping_result == 0:
            # ping成功，发送电子邮件
            send_email(subject, message, recipient)
        else:
            print("Ping失败，正在重试...")

if __name__ == "__main__":
    main()
