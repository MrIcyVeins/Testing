#!/bin/bash

# Initializare Kubernetes
echo ">>> Initializare Kubernetes Cluster"
kubeadm init --apiserver-advertise-address=172.42.42.100 --pod-network-cidr=192.168.0.0/16 >> /root/kubeinit.log 2>/dev/null

# Copiaza fisierul admin.conf in directorul .kube al utilizatorului vagrant.
# Acesta va asigura accesul utilizatorului vagrant la api-ul de kubernetes.
echo ">>> Copiaza fisierul admin.conf in directorul .kube al utilizatorului vagrant"
mkdir /home/vagrant/.kube
cp /etc/kubernetes/admin.conf /home/vagrant/.kube/config
chown -R vagrant:vagrant /home/vagrant/.kube

# Configureaza reteaua calico pe kubernetes
echo ">>> Configureaza reteaua calico pe kubernetes"
su - vagrant -c "kubectl create -f https://docs.projectcalico.org/v3.9/manifests/calico.yaml"

# Genereaza comanda pentru adaugare in cluster
echo ">>> Genereaza comanda pentru adaugare in cluster /joincluster.sh"
kubeadm token create --print-join-command > /joincluster.sh