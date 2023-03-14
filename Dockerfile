FROM ubuntu:latest

RUN     apt-get update && \
        apt-get install curl wget -y &&\
        curl --silent --location --remote-name \
        "https://github.com/kubernetes-sigs/kustomize/releases/download/kustomize/v3.2.3/kustomize_kustomize.v3.2.3_linux_amd64" && \
        chmod a+x kustomize_kustomize.v3.2.3_linux_amd64   && \
        mv kustomize_kustomize.v3.2.3_linux_amd64 /usr/local/bin/kustomize  && \
        wget -qO /usr/local/bin/yq https://github.com/mikefarah/yq/releases/latest/download/yq_linux_amd64  && \
        chmod a+x /usr/local/bin/yq