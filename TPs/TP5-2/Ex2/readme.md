
# Proposer une stack applicative « wordpress »
## Prérequis

Pour ce TP, nous aurons besoin de docker-compose. Pour l'installer :
```
sudo apt install docker-compose
```

## Démarche en cinq étapes


### 1. La récupération des images depuis le hub docker.

Pour commencer, téléchargez les dernières images Docker de MySQL et WordPress depuis Internet. Alternativement, vous pouvez charger les images depuis des fichiers avec la commande appropriée.

```
# Télécharge les dernières images Docker de MySQL et WordPress
docker pull mysql:latest
docker pull wordpress:latest
```

### 2. La création du fichier de configuration docker-compose pour le service
```
# Modification du fichier
nano docker-compose.yml
```
Le contenu du fichier docker-compose.yml doit être comme suit :
```
version: '3'
services:
wordpress:
image: wordpress:latest
ports:
- "8080:80"
environment:
WORDPRESS_DB_HOST: mysql
WORDPRESS_DB_USER: exampleuser
WORDPRESS_DB_PASSWORD: examplepassword
WORDPRESS_DB_NAME: exampledb
depends_on:
- mysql
volumes:
- wordpress_data:/var/www/html

mysql:
image: mysql:latest
environment:
MYSQL_ROOT_PASSWORD: examplepassword
MYSQL_DATABASE: exampledb
MYSQL_USER: exampleuser
MYSQL_PASSWORD: examplepassword
volumes:
- mysql_data:/var/lib/mysql

volumes:
wordpress_data:
mysql_data:
```
### 3. Le démarrage de la stack.
Commande docker-compose pour démarrer la stack en utilisant le fichier docker-compose.yml créé précédemment.
```
docker-compose up -d
```
### 4. La vérification de la disponibilité du service.
```
docker ps
```
S'assurer également de vérifier l'accessibilité de l'application via un navigateur : http://localhost:8080
### 5. L’arrêt du conteneur associé.
Arrêt de la stack complète :
```
docker-compose down
```
### Résultat
![image](https://github.com/Eken4/sa-project-tar/assets/94603692/bb5c774a-7cbb-4bcb-8d88-deb9d816e58e)

