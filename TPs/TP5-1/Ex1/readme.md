# Proposer un service
##  Prérequis
### 0. Installer Docker sur Ubuntu 

Si Docker est déjà installé sur le système, passer à l'étape 2. Sinon, faire les commandes ci-dessous :
```
sudo apt update
sudo apt install apt-transport-https ca-certificates curl software-properties-common

# Ajoutez la clé GPG du référentiel Docker
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

# Ajoutez le référentiel Docker au système
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

sudo apt update
sudo apt install docker-ce docker-ce-cli containerd.io
```
## Démarche en quarte temps
### 1. La récupération de l’image jenkins depuis le hub docker.
```
# Téléchargez la dernière image Jenkins depuis Docker Hub
docker pull jenkins
```
### 2. Le démarrage du conteneur proposant le service.
```
# Démarrez le conteneur Jenkins en exposant le port 8080
docker run -d -p 8080:8080 --name jenkins_container jenkins
```
### 3. La vérification de la disponibilité du service.
Une fois le conteneur démarré, vous pouvez accéder à Jenkins depuis votre navigateur en visitant 
http://localhost:8080. Vous devrez suivre les instructions d'initialisation pour déverrouiller Jenkins.

### 4. L’arrêt du conteneur associé.
```
# Arrêtez le conteneur Jenkins
docker stop jenkins_container
```
