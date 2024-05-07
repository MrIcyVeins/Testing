#!/bin/bash

# Configureaza fisierul hosts
echo ">>> Configureaza fisierul hosts"
cat >>/etc/hosts<<EOF
172.42.42.100 kmaster.local.net kmaster
172.42.42.101 kworker1.local.net kworker1
172.42.42.102 kworker2.local.net kworker2
EOF

# Instaleaza docker din repozitorul Docker-ce
echo ">>> Instaleaza docker-ce"
yum install -y -q yum-utils device-mapper-persistent-data lvm2 > /dev/null 2>&1
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo > /dev/null 2>&1
yum install -y -q docker-ce >/dev/null 2>&1

# Activeaza serviciul docker
echo ">>> Activeaza serviciul docker"
systemctl enable docker >/dev/null 2>&1
systemctl start docker

# Dezactiveaza SELinux si firewalld
echo ">>>  Dezactiveaza SELinux si firewalld"
setenforce 0
sed -i --follow-symlinks 's/^SELINUX=enforcing/SELINUX=disabled/' /etc/sysconfig/selinux
systemctl disable firewalld >/dev/null 2>&1
systemctl stop firewalld

# Configureaza sysctl
echo ">>> Configureaza sysctl "
cat >>/etc/sysctl.d/kubernetes.conf<<EOF
net.bridge.bridge-nf-call-ip6tables = 1
net.bridge.bridge-nf-call-iptables = 1
EOF
sysctl --system >/dev/null 2>&1

# Dezactiveaza swap
echo ">>> Dezactiveaza swap"
sed -i '/swap/d' /etc/fstab
swapoff -a

# Adauga repozitorul si instaleaza Kubernetes
echo ">>> Adauga repozitorul si instaleaza Kubernetes"
cat >>/etc/yum.repos.d/kubernetes.repo<<EOF
[kubernetes]
name=Kubernetes
baseurl=https://packages.cloud.google.com/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://packages.cloud.google.com/yum/doc/yum-key.gpg
        https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg
EOF
yum install -y -q kubeadm-1.20.15-0 kubelet-1.20.15-0 kubectl-1.20.15-0 >/dev/null 2>&1

# Porneste si activeaza serviciul kubelet
echo ">> Porneste si activeaza serviciul kubelet"
systemctl enable kubelet >/dev/null 2>&1
systemctl start kubelet >/dev/null 2>&1

# Activeaza autentificarea cu parola prin ssh
echo ">>> Activeaza autentificarea cu parola prin ssh"
sed -i 's/^PasswordAuthentication no/PasswordAuthentication yes/' /etc/ssh/sshd_config
systemctl reload sshd

# Adauga parola pentru root. Aceasta activitate este necesara pentru a putea
# automatiza procesul de adaugare noduri in cluster. Parola poate fi dezactivata
# dupa procesul initial de configurare.
echo ">>> Adauga parola pentru root"
echo "kubeadmin" | passwd --stdin root >/dev/null 2>&1
echo "export TERM=xterm" >> /etc/bashrc
