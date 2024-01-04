# Service from scratch
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
### 1. La récupération des ressources.
Téléchargez la dernière image Docker Tomcat depuis Docker Hub :
```
docker pull tomcat:9.0-jdk21-temurin-jammy
```
Téléchargez l'archive web (WAR) de Jenkins depuis le site officiel :
Vous pouvez le faire manuellement depuis le site https://jenkins.io/ ou en utilisant la commande wget :
```
wget https://updates.jenkins.io/download/war/latest/jenkins.war
```

### 2. La rédaction d’un fichier Dockerfile.
Créez un fichier nommé Dockerfile dans le même répertoire que vos ressources téléchargées :
```
# Copie l'archive WAR de Jenkins dans le répertoire webapps de Tomcat
FROM tomcat:9.0-jdk21-temurin-jammy
COPY  jenkins.war  /usr/local/tomcat/webapps/
# Expose le port d'écoute 8080
EXPOSE  8080
```
### 3. La construction de l’image.
Dans le même répertoire où se trouve votre fichier Dockerfile, exécutez la commande suivante pour construire l'image Docker :
```
docker build -t jenkins-from-scratch .
```


### 4. Le test du service.
Démarrage du conteneur :
```
docker run -d -p 8080:8080 --name jenkins_from_scratch jenkins-from-scratch
```
Vérification de la disponibilité du service :
Accédez à Jenkins depuis votre navigateur en visitant http://localhost:8080.

Arrêt du conteneur associé :
```
docker stop jenkins_from_scratch
```

## NB
Tomcat 10 et plus n'est pas supporté par Jenkins, ce qui rend l'accès à celui-ci impossible (Erreur 404).