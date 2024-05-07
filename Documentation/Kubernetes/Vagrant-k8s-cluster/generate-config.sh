# your server name goes here
target=env
server=$(kubectl config view --minify | grep server | cut -f 2- -d ":" | tr -d " ")
namespace=ns
user=user
name=$(kubectl -n ${namespace} get secrets | grep ^${user} | cut -f1 -d ' ')

ca=$(kubectl -n ${namespace} get secret/$name -o jsonpath='{.data.ca\.crt}')
token=$(kubectl -n ${namespace} get secret/$name -o jsonpath='{.data.token}' | base64 --decode)
# namespace=$(kubectl -n ${namespace} get secret/$name -o jsonpath='{.data.namespace}' | base64 --decode)

echo "
apiVersion: v1
kind: Config
clusters:
- name: default-cluster
  cluster:
    certificate-authority-data: ${ca}
    server: ${server}
contexts:
- name: default-context
  context:
    cluster: default-cluster
    namespace: ${namespace}
    user: ${user}
current-context: default-context
users:
- name: ${user}
  user:
    token: ${token}
" > ~/kube/${user}-${target}